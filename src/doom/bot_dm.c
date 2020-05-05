//////////////////////////////////
//
//  BOT_DM.C:  Deathmatch bots
//
//////////////////////////////////

#include "marshmallow.h"

 
int Marshmallow_fragSum(int playernum)
{
    int		i;
    int		frags = 0;
    
    for (i=0 ; i<MAXPLAYERS ; i++)
    {
	if (playeringame[i]
	    && i!=playernum)
	{
	    frags += players[playernum].frags[i];
	}
    }

    frags -= players[playernum].frags[playernum];

    return frags;
}


void CheckDeathmatchStatus()
{
	int i, j;
	int limit;

	// On E1M8, open the outer walls for us in DM and Sandbox
	if ( (deathmatch || Marshmallow_Sandbox) 
		&& E1M8 
		&& marshmallow_tic < 10 )
	{
		line_t junk;
		junk.tag = 666;

		EV_DoFloor (&junk, lowerFloorToLowest);
	}
	
	if (!deathmatch || gametic < 50)
		return;

	if ( !dm_timelimit )
		dm_timelimit = 10*MINUTE;

	if ( !dm_fraglimit )
		dm_fraglimit = 50;
	
	// fraglimit hit?
	for (i=0 ; i<MAXPLAYERS ; i++)
	{
	    if (playeringame[i])
	    {
		for (j=0 ; j<MAXPLAYERS ; j++)
		    if (playeringame[j])
			dm_frags[i][j] = players[i].frags[j];

		dm_scores[i] = Marshmallow_fragSum(i);
		level_stats.dm_frags = dm_scores[consoleplayer]; // NEW

		if (dm_scores[i] >= dm_fraglimit)
		{
			SHOW_MESSAGE "FRAG LIMIT HIT!";
			level_stats.dm_frags = dm_scores[consoleplayer];  // NEW
			G_ExitLevel();  
		}
	    }
	}
	
	// timelimit hit?
	limit = dm_timelimit * MINUTE;
	if (marshmallow_tic >= limit)
	{
		SHOW_MESSAGE "TIME LIMIT HIT!";
		level_stats.dm_frags = dm_scores[consoleplayer];  // NEW
		G_ExitLevel();  
	}
}


#define TARGETING_DISTANCE MISSILERANGE*0.75

static boolean DistanceCheck(mobj_t* actor, mobj_t* target)
{
	fixed_t dist;

	dist = P_AproxDistance( actor->x - target->x,
							actor->y - target->y );

	if (dist > TARGETING_DISTANCE)
	{
		if (debugmode)
		SHOW_MESSAGE "TARGET TOO FAR AWAY!";
		
		return false;
	}
	else
		return true;
}


void Deathmatch_Bot_ChooseTarget(int bot)
{
//	int p;

	if (this_Bot.state == ENGAGING_ENEMY)
		return;

	if (marshmallow_tic < 50)   // wait a bit at the beginning of the map to start shooting
		return;

	//if (Bot_CheckTargetingDelay(bot))  // try using this
	//	return;

	if ( Deathmatch_PlayerInSight(bot) );  // NEW if
		return;  // NEW

	// defend self
	if (this_Bot.player->attacker
		&& this_Bot.player->attacker->health > 0)
	{
		this_Bot.foundmonster = this_Bot.player->attacker;
		Bot_TakeTarget(bot);
	}

	// find monsters    [NEW 2-17-2020]
	if (nomonsters == false)
		Bot_ChooseTarget(bot);
}


void Bot_Melee(int bot)
{
    angle_t	angle;
    int		damage;
    int		slope;

    //if (!player) return; // [crispy] let pspr action pointers get called from mobj states

    damage = 2*(P_Random ()%10+1) * chainsaw_damage_multiplier;  // [marshmallow]

    angle = thisBot_mo->angle;
    angle += P_SubRandom() << 18;
    
    // use meleerange + 1 se the puff doesn't skip the flash
    slope = P_AimLineAttack (thisBot_mo, angle, MELEERANGE+1);
    P_LineAttack (thisBot_mo, angle, MELEERANGE+1, slope, damage);

    //A_Recoil (player);

    //if (!linetarget)
    //{
	//S_StartSound (thisBot_mo->player->so, sfx_sawful); // [crispy] weapon sound source
	//return;
   // }

    S_StartSound (thisBot_mo, sfx_sawful);  // used to be sfx_sawhit
	P_SetMobjState(thisBot_mo, thisBot_mo->info->missilestate);  // TESTING
}


void Deathmatch_Bot_Attack(int bot)
{
	mobj_t* actor = players[bot].mo;
	bot_skill_t skill = this_Bot.skill;

	if (this_Bot.state != ENGAGING_ENEMY)
		return;

	if (actor->target->health <= 0
		|| actor->health <= 0) // NEW line
	{
		Bot_SetState(SEARCHING_FOR_NODES, bot);  
		return;
	}
	
	if (Bot_CheckTargetTimeout(bot))
	{
		if (debugmode)
		SHOW_MESSAGE "CAN'T HIT TARGET!";
		
		Bot_SetState(SEARCHING_FOR_NODES, bot);

		return;
	}
	
	if (!P_CheckSight(actor, actor->target))
	{
		Bot_Chat(bot, "LOST YOU!");

		//if ( Deathmatch_SearchForNodes(&main_path, actor) )  // TODO:  search for player's newest path first, then if not found, fall back on any other path
		if ( Deathmatch_SearchForNodesFromEnd(&main_path, actor) ) // TESTING...
		{
			Bot_SetState(DEATHMATCH_ROAM_PATH, bot);
		}
		else
		{
			Bot_SetState(SEARCHING_FOR_NODES, bot);  
		}
	}

	if ( this_Bot.weapon == BOT_CHAINGUN )
		skill = skill / 2;  // make them less accurate when they are using chaingun

	//if ( P_CheckMeleeRange(actor) )
	//	skill = 99;

	//if ( PercentChance(skill) )  // this should make them less accurate
		A_FaceTarget(actor);

	//if ( PercentChance(skill) )
	//	Bot_Walk(bot);  // let's try it without this
	//else
	//	Bot_Stop(bot);

	if (P_CheckMeleeRange(thisBot_mo))
	{
		S_StartSound (thisBot_mo, sfx_sawful);

		if ( gameskill >= sk_medium )
			skill = skill/2;    // bots were a little too good with the chainsaw
		
		if ( PercentChance(skill) ) 
			Bot_Melee(bot);
		
		return;
	}

	if ( PercentChance(skill) )
		Bot_Shoot(bot);
}


boolean Deathmatch_TargetInView(mobj_t* actor, mobj_t* target)
{
	angle_t an;

	an = R_PointToAngle2 (actor->x,
				actor->y, 
				target->x,
				target->y)
			- actor->angle;
	    
	if (an > ANG90 && an < ANG270)   // target is behind actor
		return false;
	else
		return true;
}


int Deathmatch_PlayerInSight(int bot)
{
	int p;
	mobj_t* actor = players[bot].mo;

	// so we don't always start looking for player 0 first
	//rand = GetRandomIntegerInRange(0, 3);

	if (this_Bot.state == ENGAGING_ENEMY)  // NEWWWW!!!!!! TESTING!!!!!!!!!
		return 0;  // should be zero, right?

	for (p = GetRandomIntegerInRange(0, 3); p < MAXPLAYERS; p++)
	{
		mobj_t* target = players[p].mo;
		player_t* player = &players[p];

		if (!playeringame[p])
			continue;

		if (actor == target)
			continue;

	//	if (target == this_Bot.blacklisted_target)  // NEW!!!!!!!!
	//		continue;

		if (P_CheckSight(actor, target)
			&& Deathmatch_TargetInView(actor, target)   // NEW TESTING!
			&& DistanceCheck(actor, target)   // EVEN NEWER!!!!!!!!!
			&& target->health > 0
			&& !PLAYER_INVISIBLE)
		{
			this_Bot.foundmonster = players[p].mo;
			Bot_TakeTarget(bot);
			//thisBot_mo->current_node = 0;  // NEW
			Bot_Chat(bot, "FOUND YOU!");

			return 1;
		}
	} 

	return 0;
}


int Deathmatch_SearchForNodes(navpath_t* path, mobj_t* actor)
{
	int node;	
	int max = path->total_nodes;

	for ( node = 0; node </*=*/ max; node++ )   // TODO: make sure this change is needed
	{
		if ( Path_FoundNode(path, actor, node) )
		{
			actor->potential_node = node;

			return node;  // NEW testing
		}
	}

	//return actor->potential_node;	  // NEW removed testing without this

	return 0;
}


int Deathmatch_SearchForNodesFromEnd(navpath_t* path, mobj_t* actor)
{
	int node;	
	int max = path->total_nodes;

	for ( node = max; node >/*=*/ 0; node-- )   // TODO: make sure this change is needed
	{
		if ( Path_FoundNode(path, actor, node) )
		{
			actor->potential_node = node;

			return node;  // NEW testing
		}
	}

	//return actor->potential_node;	  // NEW removed testing without this

	return 0;
}


int Deathmatch_DetermineNextNode(navpath_t* path, mobj_t* actor)
{
	int	curr_node, next_node, potential_node;
	int bot;

	if ( IsBot(actor->player) )
		bot = IsBot(actor->player);
	else
		return 0;  // should be zero, right?

	curr_node = actor->current_node;  
	potential_node = actor->potential_node;

	switch (this_Bot.state)
	{
		case DEATHMATCH_ROAM_PATH:

			next_node = curr_node + 1;  
											
			if ( potential_node > next_node 
				 && Path_IsNodeVisible(potential_node, path, actor) 
				 /*&& IsNodeWalkable(actor)*/ )  // NEW!!!!
			{
				next_node = potential_node;
			}

			return next_node;

		case DEATHMATCH_ROAM_REVERSEPATH:

			next_node = curr_node - 1;  

			if ( potential_node < next_node 
				 && Path_IsNodeVisible(potential_node, path, actor) 
				 /*&& IsNodeWalkable(actor)*/ )  // NEW!!!!
			{
				next_node = potential_node;
			}

			return next_node;

		default:

			return 0;
	}
}


void Deathmatch_FollowPath(navpath_t* path, mobj_t* actor)  
{

	int next_node;
	int	bot;	
	
	if ( IsBot(actor->player) )
		bot = IsBot(actor->player);
	else
		return;

	next_node = Deathmatch_DetermineNextNode(&main_path, actor);

	if (path->dm_deathnode[ next_node ])     
	{
		if (debugmode)
		SHOW_MESSAGE "DEATH NODE HIT!";
		Bot_SetState(DEATHMATCH_ROAM_REVERSEPATH, bot);

		return;
	}

	if ( next_node == 0 )
	{
		if (debugmode)
		SHOW_MESSAGE "BEGINNING OF PATH HIT!";
		Bot_SetState(DEATHMATCH_ROAM_PATH, bot);   

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
		actor->angle = R_PointToAngle2 (actor->x,
										actor->y,
										path->x[next_node],
										path->y[next_node]);
	}
}


void Deathmatch_CheckForProblems(int bot)  
{
	int curr_node, prev_node;
	mobj_t* actor = players[bot].mo;

	if (this_Bot.stuck_timeout)
	{
		this_Bot.stuck_timeout--;
		return;
	}
	else
	{
		Bot_ResetStuckTimeout(bot);
	}

	prev_node = actor->previous_node;  

	switch (this_Bot.state)
	{
		
	case DEATHMATCH_ROAM_PATH:

		curr_node = actor->current_node;
			
		if (curr_node > prev_node)
		{
			actor->previous_node = curr_node;
			return;
		}
			
		if (curr_node == prev_node)
		{
			Bot_Chat(bot, BOTCHATSTUCK);
			Bot_SetState(TRYING_TO_GET_UNSTUCK, bot);
		}
					
		break;

	case DEATHMATCH_ROAM_REVERSEPATH:

		curr_node = actor->current_node;
			
		if (curr_node < prev_node)
		{
			actor->previous_node = curr_node;
			return;
		}
			
		if (curr_node == prev_node)
		{
			Bot_Chat(bot, BOTCHATSTUCK);
			Bot_SetState(TRYING_TO_GET_UNSTUCK, bot);
		}
					
		break;

	case TRYING_TO_GET_UNSTUCK:

		curr_node = Deathmatch_SearchForNodes(&main_path, actor); // actor->potential_node;  

		if (curr_node
			&& IsNodeWalkable(actor)) 
		{
			actor->current_node = curr_node;
			Bot_SetState(DEATHMATCH_ROAM_PATH, bot);
		}

		break;
	}
}


#define DROPPED_ITEM_CLEANUP_TIME 525 // 525 tics = 15 seconds

void DroppedItemCleanup()
{
    thinker_t*	th;
    mobj_t*	mo;

	if ( !deathmatch || realnetgame )
		return;

    for (th = thinkercap.next ; th != &thinkercap ; th=th->next)
    {
		if (th->function.acp1 != (actionf_p1)P_MobjThinker)
			continue;
	
		mo = (mobj_t *)th;
		if ( mo->drop_tic )
		{
			if ( gametic - mo->drop_tic > DROPPED_ITEM_CLEANUP_TIME )
			{
				P_RemoveMobj(mo);
			}
		}
    }
}


void CorpseCleanup()
{
    thinker_t*	th;
    mobj_t*	mo;

	//if ( !deathmatch )  // cleaning up player corpses still gives us weird issues
		return;

    for (th = thinkercap.next ; th != &thinkercap ; th=th->next)
    {
		if (th->function.acp1 != (actionf_p1)P_MobjThinker)
			continue;
	
		mo = (mobj_t *)th;
		if ( mo->death_tic )
		{
			if ( gametic - mo->death_tic > DROPPED_ITEM_CLEANUP_TIME )
			{
				P_RemoveMobj(mo);
			}
		}
    }
}


void Deathmatch_Bot_AI(int bot)
{
	Bot_Think(bot);

	switch (this_Bot.state)
	{
	case JUST_SPAWNED:

		Bot_StartPatrolling(bot);

		break;
	
	case SEARCHING_FOR_NODES:

		if ( Deathmatch_SearchForNodes(&main_path, thisBot_mo) ) 
			Bot_SetState(DEATHMATCH_ROAM_PATH, bot );

		break;

	case DEATHMATCH_ROAM_PATH:
	case DEATHMATCH_ROAM_REVERSEPATH:  

		if ( thisBot_mo->current_node < main_path.total_nodes
			/*&& thisBot_mo->current_node > 0*/ )					// so nodes don't go negative 
		{
			Deathmatch_FollowPath(&main_path, thisBot_mo);
		}
		else
		{
			Bot_SetState(SEARCHING_FOR_NODES, bot);  
		}

		break;
	}

	if (this_Bot.state != ENGAGING_ENEMY)
		this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT*3;   // NEW TESTING
	
	Bot_Meander(bot);

	Deathmatch_Bot_Attack(bot);
	Deathmatch_Bot_ChooseTarget(bot); 

	Bot_UseStuff(bot); 

	Deathmatch_CheckForProblems(bot); 
}
