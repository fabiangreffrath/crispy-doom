////////////////////////////////////////////
//
//  NAVIGATION.C:  Bot waypoint navigation
//
////////////////////////////////////////////

#include "marshmallow.h"


void Path_Follow(navpath_t* path, mobj_t* actor)
{
	int next_node, death_node;
	int	bot;	
	
	if ( IsBot(actor->player) )
		bot = IsBot(actor->player);
	else
		return;

	death_node = main_path.death_node;  

	next_node = Path_DetermineNextNode(&main_path, actor);

	if (next_node == death_node)     
	{
		// TODO: reverse path

		if ( deathmatch )
		{
			Bot_StartPatrolling( bot );
			//Bot_SetState(HOLDING_POSITION, bot);
		}
		else
		{
			Bot_WaitForPlayer(bot);

			//Bot_SetState(FOLLOWING_PATH_REVERSE, bot);  // TESTING REVERSE PATH IN COOP

			Bot_MarkDeathNode(bot);  
		}

		return;  
	}

	if ( Path_FoundNode(path, actor, next_node) )  
	{
		actor->previous_node = actor->current_node; 
		actor->current_node = next_node;

		Bot_ResetStuckTimeout(bot);  
	}
	else
	{
		// TODO: NEW TESTPATH EXPERIMENT HERE

		actor->angle = R_PointToAngle2 (actor->x,
										actor->y,
										path->x[next_node],
										path->y[next_node]);
	}
}


boolean Path_CheckForUseNode(navpath_t* path, int curr_node, int potential_node)
{
	int i, end;

	i = curr_node;
	end = potential_node;

	if ( !specialnodes )
		return false;

	for ( ; i < end ; i++ )
	{
		if ( path->use_node[i] )
			return true;
	}

	return false;
}


int Path_DetermineNextNode(navpath_t* path, mobj_t* actor)
{
	int	curr_node, next_node, potential_node;
	int bot;

	if ( IsBot(actor->player) )
		bot = IsBot(actor->player);
	else
		return 0;

	curr_node = actor->current_node;  
	potential_node = actor->potential_node;

	switch (this_Bot.state)
	{
		case FOLLOWING_MAIN_PATH:
		case DEATHMATCH_ROAM_PATH:
		case AWAITING_TARGET:	

			next_node = curr_node + 1;  
											
			if ( potential_node > next_node 
				 && Path_IsNodeVisible(potential_node, path, actor) 
				 && !Path_CheckForUseNode(path, curr_node, potential_node) )  // If the next segment includes a use node, don't skip ahead
			{
				next_node = potential_node;
			}

			return next_node;

		case FOLLOWING_PATH_REVERSE:
		case DEATHMATCH_ROAM_REVERSEPATH:

			next_node = curr_node - 1;  

			if ( potential_node < next_node 
				 && Path_IsNodeVisible(potential_node, path, actor) )
			{
				next_node = potential_node;
			}

			return next_node;

		default:
			return 0;
	}
}


int Path_SearchForNodes(navpath_t* path, mobj_t* actor)
{
	int node;	
	int max = path->total_nodes;

	for ( node = 0; node <= max; node++ )   
	{
		if ( Path_FoundNode(path, actor, node) )
			actor->potential_node = node;
	}

	return node;  // should be actor->potential_node;	
}


void Path_Plot(navpath_t* path, mobj_t* actor)
{
	if (path->node_timer)
	{
		path->node_timer--;
		return;
	}
	else
		path->node_timer = NODE_TIMEOUT;

	if (!Path_CheckNodeSpacing(path, actor, path->total_nodes))  
		return;
	else
		Path_DropNode(path, actor);
}


int Path_DropNode(navpath_t* path, mobj_t* actor)
{
	int new_node;
	int marker = waypoint_icon;

	path->total_nodes++;
	new_node = path->total_nodes;

	path->x [ new_node ] = actor->x;
	path->y [ new_node ] = actor->y;
	path->z [ new_node ] = actor->z;

	path->angle [ new_node ] = actor->angle;

	if (gamekeydown[key_q])
	{
	path->sprint_node [ new_node ] = true;
	marker = DOOM1_POOL_OF_BLOOD;
	}

	if (gamekeydown[key_use])
	{
	path->use_node[ new_node ] = true; 
	marker = DOOM1_POOL_OF_BLOOD;
	}

	if (path->show_markers)
	{
		path->marker[ new_node ] 
			 = P_SpawnMobj(path->x[ new_node ], 
						   path->y[ new_node ], 
						   ONFLOORZ, 
						   marker);
	}

	return new_node;
}


void Path_HandleDeathNode()  
{
	if ( PlayerIsDead() )   
	{
		int death_node, total_nodes;

		total_nodes = main_path.total_nodes;
		death_node = main_path.death_node;

		if (total_nodes < NODE_START_OFFSET)   
			return;

		if (deathmatch)
		{
			main_path.dm_deathnode[ total_nodes ] = true;  // NEW
			return;
		}
		
		if (main_path.total_nodes > death_node)
		{
			if (debugmode)
				SHOW_MESSAGE "DEATH NODE DROPPED";

			// Coop:
			main_path.death_node = Path_DropNode(&main_path, MAIN_PLAYER.mo);
		}
	}
}


void Path_TrackPlayerPosition(navpath_t* path, mobj_t* actor)
{
	int nearby_node;  
	
	nearby_node = Path_SearchForNodes(&main_path, actor);   

	if (nearby_node)
		actor->current_node = nearby_node;
}


void Path_PlayerNodes()
{
	if ( realnetgame )   
		return;

	Path_HandleDeathNode();

	// Drop nodes while we walk, except when we're dead
	if ( !PlayerIsDead() )    
		Path_Plot(&main_path, MAIN_PLAYER.mo);  

//	Path_TrackPlayerPosition(&main_path, MAIN_PLAYER.mo);  // TODO: not using this yet
}


//
//  UTITLITY FUNCTIONS
//

boolean Path_CheckNodeSpacing(navpath_t* path, mobj_t* actor, int node)  
{
	int dist;

	dist = P_AproxDistance (actor->x - path->x[node], 
							actor->y - path->y[node]);  

	if (!dist)
		return false;  

	if (dist < NODE_SPACING_DISTACE)   
		return false;
	else
		return true;
}


boolean Path_FoundNode(navpath_t* path, mobj_t* actor, int node)  
{
	int dist;

	dist = P_AproxDistance (actor->x - path->x[node], 
							actor->y - path->y[node]);

	if (dist < NODE_TOUCH_RADIUS) 
		return true;
	else
		return false;
}


boolean Path_CloseToEnd(navpath_t* path, mobj_t* actor)
{
	if (actor->current_node < path->total_nodes - END_OF_CHAIN_OFFSET)  
		return false;
	else
		return true;
}


boolean Path_CloseToEndReverse(navpath_t* path, mobj_t* actor)
{
	if (actor->current_node > path->total_nodes)
		return false;
	else
		return true;
}


boolean Path_IsNodeVisible(int node, navpath_t* path, mobj_t* actor)
{
	mobj_t* t1;
	mobj_t* t2;
				
	t1 = P_SpawnMobj(actor->x, actor->y, ONFLOORZ, SIGHT_TEST);
	t2 = P_SpawnMobj(path->x[node], path->y[node], ONFLOORZ, SIGHT_TEST);

	if (!P_CheckSight(t1, t2))  // don't grab waypoints through walls
	{
		if (debugmode)
			SHOW_MESSAGE "REJECTING OUT-OF-SIGHT NODE.";

		P_RemoveMobj(t1);
		P_RemoveMobj(t2);

		return false;
	}
	else
	{
		if (debugmode)
			SHOW_MESSAGE "FOUND VISIBLE NODE.";

		P_RemoveMobj(t1);
		P_RemoveMobj(t2);

		return true;
	}
}


boolean IsNodeWalkable(mobj_t *actor) 
{
	const int height_limit = mobjinfo[MT_PLAYER].height / 2;
	int curr_node = actor->potential_node;  
	int result = main_path.z[curr_node] - actor->z;
	
	if (result > height_limit)
	{
		if (debugmode)
			SHOW_MESSAGE "REJECTING UNWALKABLE NODE.";

		return false;
	}
	else
	{
		return true;
	}
}


boolean Path_CheckNodeZ(navpath_t* path, int node1, int node2)  // not used
{
	// as-is, this doesn't work well with stairs

	if (path[node1].z == path[node2].z)
		return true;
	else
	{
		if (debugmode)
			SHOW_MESSAGE "REJECTING UN-WALKABLE NODE.";

		return false;
	}
}


void Path_ClearDeathNode()
{
	int	 i;

	for (i = 0; i < BotsInGame; i++)
		Bots[i].found_deathnode = false;
	
	main_path.death_node = NULL;

	if (debugmode)
		SHOW_MESSAGE "DEATH NODE CLEARED";
}


//
// WIP
//

#define SPACING_INTERVAL 5000000

navpath_t Path_GenerateSegment(mobj_t* actor)
{
	angle_t		an;
	fixed_t		x, y;
//	int			i;
	int			new_y = 0;
	const int	max_nodes = 5;

//	for (i=0; i<max_nodes; i++)
//	{
		mobj_t* node;
		
		new_y += SPACING_INTERVAL;

		an = actor->angle >> ANGLETOFINESHIFT;

		x = actor->x + FixedMul (24*FRACUNIT, finecosine[an]);
		y = actor->y + FixedMul (24*FRACUNIT, finesine[an]);// + MARSHMALLOW_DROPOBSTACLE_OFFSET_Y + new_y;  // Negative to make it go behind you?

		node = P_SpawnMobj(x, y, ONFLOORZ, MT_CLIP);
//	} 
}


