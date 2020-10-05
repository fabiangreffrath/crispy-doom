////////////////////////////////////////////
//
//  BOT_LIB.C:  Utility functions for bots
//
////////////////////////////////////////////

#include "bot.h"


//
//  ACTIONS/BEHAVIORS
//

void Bot_Shoot(int bot)
{
	mobj_t* actor;
	player_t* plr;

	actor = players[bot].mo;
	plr = &players[bot];

	//if (plr->readyweapon == wp_chainsaw)
    //    this_Bot.weapon_switch_delay = WEAPON_SWITCH_DELAY;

	// slow down rate of fire a bit
	if (this_Bot.firingDelay
	    && this_Bot.player->readyweapon != wp_chainsaw)  // TESTING 2020
	{
		this_Bot.firingDelay--;
		return;
	}

	if (actor->health <= 0)
		return;

	this_Bot.turn_delay = BOT_TURN_DELAY;  // reset turn delay so that firing animation finishes before turning back in player's direction

	P_NoiseAlert (actor, actor);  // so monsters can hear us
		
	switch (this_Bot.weapon)
	{
	case BOT_PISTOL:
		A_FirePistol(NULL, plr, NULL);
		S_StartSound (actor, sfx_pistol);
		this_Bot.firingDelay = BOT_FIRINGDELAY_PISTOL;
		this_Bot.player->readyweapon = wp_pistol;
		break;

	case BOT_SHOTGUN:
		A_FireShotgun(NULL, plr, NULL);
		S_StartSound (actor, sfx_shotgn);
		this_Bot.firingDelay = BOT_FIRINGDELAY_SHOTGUN;
		this_Bot.player->readyweapon = wp_shotgun;
		break;

	case BOT_SUPERSHOTGUN:
		A_FireShotgun2(NULL, plr, NULL);
		S_StartSound (actor, sfx_dshtgn);
		this_Bot.firingDelay = BOT_FIRINGDELAY_SUPERSHOTGUN;
		this_Bot.player->readyweapon = wp_supershotgun;
		break;

	case BOT_CHAINGUN:
		P_SetMobjState (actor, S_PLAY_ATK2); 
		//A_FireCGun(NULL, plr, NULL);  
	    P_BulletSlope (plr->mo);
		P_GunShot (plr->mo, !plr->refire);
		A_Recoil (plr);

		S_StartSound (actor, sfx_pistol);
		this_Bot.firingDelay = BOT_FIRINGDELAY_CHAINGUN;
            this_Bot.player->readyweapon = wp_chaingun;
		break;

	case BOT_MISSILE:
		P_SetMobjState (actor, S_PLAY_ATK2);
		P_SpawnPlayerMissile (actor, MT_ROCKET); 
		this_Bot.firingDelay = BOT_FIRINGDELAY_MISSILE;
		this_Bot.player->readyweapon = wp_missile;
		break;

	case BOT_PLASMA:
		P_SetMobjState (actor, S_PLAY_ATK2);
		P_SpawnPlayerMissile (actor, MT_PLASMA);
		this_Bot.firingDelay = BOT_FIRINGDELAY_PLASMA;
		this_Bot.player->readyweapon = wp_plasma;
		break;

	case BOT_GREENPLASMA:
		P_SetMobjState (actor, S_PLAY_ATK2);
		P_SpawnPlayerMissile (actor, MT_ARACHPLAZ);
		this_Bot.firingDelay = BOT_FIRINGDELAY_PLASMA;
		this_Bot.player->readyweapon = wp_plasma;
		break; 

	case BOT_BFG:
		P_SetMobjState (actor, S_PLAY_ATK2); 
		S_StartSound (actor, sfx_bfg);
		P_SpawnPlayerMissile (actor, MT_BFG);
		this_Bot.firingDelay = BOT_FIRINGDELAY_BFG;
		this_Bot.player->readyweapon = wp_bfg;
		break;
	}
}


void Bot_PushBarrel(int bot)
{
	unsigned	ang;
	fixed_t		thrust =  BARREL_PUSH_THRUST;
	
	if (!players[bot].touching_barrel || !Marshmallow_BarrelPushing)  
		return;

	ang = players[bot].mo->angle;

	ang >>= ANGLETOFINESHIFT;
	players[bot].current_barrel->momx += FixedMul (thrust, finecosine[ang]);
	players[bot].current_barrel->momy += FixedMul (thrust, finesine[ang]);
}


void Bot_StartFollowingPlayer(int bot)  
{	
	// NOTE: don't do any sight checks in here

	if (this_Bot.state == FOLLOWING_CLOSE  // only come in here if we're not already following
		//|| this_Bot.state == AWAITING_TARGET  // this was preventing us from cycling orders back to FOLLOW mode
		/*|| this_Bot.state == ENGAGING_ENEMY*/)  // NEW!!!
		return;

	if (deathmatch)
	{
		Bot_SetState(SEARCHING_FOR_NODES, bot);  
		//this_Bot.orders = FOLLOW_LEADER;  
		return;
	}

	players[bot].mo->target = MAIN_PLAYER.mo;    
	Bot_FacePlayer(players[bot].mo); 
	Bot_SetState(FOLLOWING_CLOSE, bot);
	//Bots[bot].targetingDelay = BOT_TARGET_DELAY;  // NOTE: recently removed Oct 2018

	//Bot_Chat(bot, BOTCHATFOLLOW);  // this really does step on other messages a lot
}


void Bot_WaitForPlayer(int bot)
{
	Bot_SetState(HOLDING_POSITION, bot);  // TODO: make this WAITING_FOR_PLAYER

	Bot_Chat(bot, BOTCHATWAIT);
	//SHOW_MESSAGE DEH_String(ORDERMSGHOLD);   // REMOVED TEMP
}


void Bot_StartPatrolling(int bot)
{
	Bot_Chat(bot, "I'LL PATROL HERE!");  // used to be "I'LL SECURE THIS AREA!" but was too long
	//SHOW_MESSAGE DEH_String(ORDERMSGPATROL);  
		
	Bot_SetState(SEARCHING_FOR_NODES, bot);
	this_Bot.orders = PATROLLING;
}


// NOTE: this is currently unused since we made target assignment apply to all bots
void Bot_AssignTarget(int bot)
{
	this_Bot.orders = FOLLOW_CURSOR;
	Bot_SetState(AWAITING_TARGET, bot);
	lastbot = bot;

	Bot_Stop(bot);  
	
	SHOW_MESSAGE DEH_String(ORDERSMSGTAKE);
					
	botcursor_on = true;  
	crispy->crosshair = 2;
}


void Bot_CycleOrders()
{
	if (spawntics < 50)  // so we don't erroneously issue bot orders while mashing use key to respawn
		return;

	if (CheckKeyDelay() || MAIN_PLAYER.playerstate != PST_LIVE)
		return;
	else
		menu_key_delay = 7;   // should be ResetKeyDelay()

    PlayRadioNoise();

	Bots[viewingbot].orders++;

	if (Bots[viewingbot].orders == MAX_ORDERS)
		Bots[viewingbot].orders = 1;

	switch (Bots[viewingbot].orders)
	{
		case FOLLOW_LEADER:

			//SHOW_MESSAGE DEH_String(ORDERMSGFOLLOW);  

			Bot_Chat(viewingbot, BOTCHATFOLLOW); // new

			//if (P_CheckSight(players[viewingbot].mo, MAIN_PLAYER.mo))
				Bot_StartFollowingPlayer(viewingbot);
			//else
			//	Bot_StartFollowingNearestPath(viewingbot);
			break;

		case PATROLLING:
			Bot_StartPatrolling(viewingbot);
			break;

		case HOLD_POSITION:
			Bot_WaitForPlayer(viewingbot);
			break;

		case FOLLOW_CURSOR:
			Bot_AssignSingleBotTarget();
			//Bot_AssignTarget(viewingbot);
			break;
	}
}


void Bot_AssignTargetToAll()
{
	playerindex_t bot;
				
	for (bot = BOT_1; bot < MAX_BOTS; bot++)  // removed <= since we might have been going one player over
	{
		if (!bot_in_game[bot])  // this was to try and stop the screen flicker but didn't affect it
			continue;

		if ( PlayerIsDead() )
			return;

		this_Bot.foundmonster = linetarget;  
		Bot_TakeTarget(bot);
		this_Bot.orders = FOLLOW_LEADER;  // NOTE: might need new orders SEEK_TARGET or something
	}

	SHOW_MESSAGE DEH_String(ORDERSMSGASSIGN);
    PlayRadioNoise();
}


void Bot_AssignSingleBotTarget()
{
	int bot = lastbot;

	//if (CheckKeyDelay())  // isn't this redundant since we do it in calling function?
	//	return;
	//else
	//	menu_key_delay = 7; 

    PlayRadioNoise();

	if (!botcursor_on)
	{
		botcursor_on = true;
		crispy->crosshair = 2;
	}
	else
	{
		P_AimLineAttack(MAIN_PLAYER.mo, MAIN_PLAYER.mo->angle, MISSILERANGE);

		if (linetarget)
		if (linetarget->type != MT_PLAYER)
		{
			this_Bot.player->mo->target = linetarget; 

			//this_Bot.foundmonster = linetarget;
			//Bot_TakeTarget(bot);

			if (P_CheckSight(players[lastbot].mo, linetarget) )
			{
				A_FaceTarget (this_Bot.player->mo);  
				Bot_SetState(ENGAGING_ENEMY, bot);
					
				//players[lastbot].mo->target = linetarget;
			}
			else
			{
				Bot_StartFollowingNearestPath(bot);
					
			}
		}

		botcursor_on = false;
		crispy->crosshair = 0;
		//Bots[lastbot].orders = FOLLOW_LEADER;  // back to the first one
	}
}


void Bot_TakeTarget(int bot)
{
	if (this_Bot.foundmonster == this_Bot.blacklisted_target
		&& this_Bot.blacklist_timeout)
			return;
	
	if (!deathmatch)
	Bot_Chat(bot, BOTCHATENEMYSPOT);

	Bot_SetState(ENGAGING_ENEMY, bot);

	this_Bot.player->mo->target = this_Bot.foundmonster; 
	A_FaceTarget (this_Bot.player->mo);  

	//if (!deathmatch)
	this_Bot.target_lasthealth = players[bot].mo->target->health;
	
	this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT;

	this_Bot.cached_node = players[bot].mo->current_node;  // NEW!  for temp path while attacking; not used yet
}


navpath_t* Bot_StartFollowingNearestPath(int bot)
{
//	int nearest_node;
	mobj_t* actor = players[bot].mo;

	if (Path_SearchForNodes(&main_path, actor))
		Bot_SetState(FOLLOWING_MAIN_PATH, bot);

#if 0

	if (nearest_node = Path_SearchForNodes(&main_path, actor)) 
	{
		//SHOW_MESSAGE "MAIN PATH FOUND!";
		actor->current_node = nearest_node;
		Bot_SetState(FOLLOWING_MAIN_PATH, bot);
	}

	else if (nearest_node = Path_SearchForNodes(&this_Bot.attack_path, actor))
	{
		SHOW_MESSAGE "ATTACK PATH FOUND!";
		actor->current_node = nearest_node;
		Bot_SetState(FOLLOWING_MY_PATH_REVERSE, bot);
	}
#endif

	else
	{
		if (debugmode)
		SHOW_MESSAGE "NO NODES FOUND!";
		
		//Bot_StartPatrolling(bot);  
	}

	//return 0; // should fix compiler warning
}


void Bot_HoldPosition()  // rename to Bot_OrderToStop or similar
{
	bot_t bot;

	for (bot=BOT_1;bot<=BotsInGame;bot++)
	{
		if (!bot_in_game[bot])
			continue;

		Bot_SetState(HOLDING_POSITION, bot);
		this_Bot.orders = HOLD_POSITION;
	}
}


void Bot_Run(int bot)  // only used in dmbot code
{
	players[bot].cmd.forwardmove = forwardmove[BOT_RUN_SPEED];
}


void Bot_Sprint(int bot)  
{
	players[bot].mo->angle = main_path.angle[players[bot].mo->current_node/*+1*/];  // ISSUE: getting bad angles sometimes when sprinting
	players[bot].cmd.forwardmove = forwardmove[BOT_RUN_SPEED] * 1.5;
}


void Bot_Walk(int bot)
{
	int bot_speed;

	switch (default_bot_speed)
	{
	case BOTS_CAREFUL:
		bot_speed = forwardmove[BOT_WALK_SPEED] * 0.5; 
		break;

	case BOTS_WALK:
		bot_speed = forwardmove[BOT_WALK_SPEED];
		break;

	case BOTS_RUN:
		bot_speed = forwardmove[BOT_WALK_SPEED] * 1.5; 
		break;
	}

	if (!deathmatch)
	{
		if (gamekeydown[key_speed])
			bot_speed = forwardmove[BOT_RUN_SPEED];
	}
	else  // deathmatch only
	{
		bot_speed = forwardmove[BOT_WALK_SPEED] * 1.5;   // NEW faster bots in DM
	}
	
	players[bot].cmd.forwardmove = bot_speed;  
}


void Bot_Stop(int bot)
{
	players[bot].cmd.forwardmove = BOT_STOP_MOVING; 
	players[bot].cmd.sidemove = BOT_STOP_MOVING; 
}


void Bot_FacePlayer(mobj_t* actor)
{
	int bot = IsBot(actor->player);
	
	if (this_Bot.turn_delay)  // rethink this; this timeout stuff should be somewhere else
	{
		this_Bot.turn_delay--;
		return;
	}
	else
		this_Bot.turn_delay = BOT_TURN_DELAY;

	if (!P_CheckSight(actor, MAIN_PLAYER.mo))  
		return;

	actor->angle = R_PointToAngle2 (actor->x,
									actor->y,
									MAIN_PLAYER.mo->x,
									MAIN_PLAYER.mo->y);
}


void Bot_UseStuff(int bot)
{
	int			angle, i;
    fixed_t		x1;
    fixed_t		y1;
    fixed_t		x2;
    fixed_t		y2;
	mobj_t*		actor;

	if (players[bot].playerstate != PST_LIVE)
		return;

	actor = players[bot].mo;

	if (!actor)
		return;
//#if 0
	if ( main_path.use_node[ actor->current_node ] )

	goto skip;   // TEMP
//#endif
	for (i=0; i<=BotsInGame; i++)
	{
		if (i == bot)
			break;   // if this is the bot who is waiting_for_door, continue through the rest of this function

		if (Bots[i].waiting_for_door)
			return;  // if any other bots are waiting for a door, it might be this one, so don't use anything yet
	}

	skip:   // TEMP

	if (usedelay)
		return;

	
	// NOTE: skip was here

    usething = actor;
		
    angle = actor->angle >> ANGLETOFINESHIFT;

    x1 = actor->x;
    y1 = actor->y;
    x2 = x1 + (USERANGE>>FRACBITS)*finecosine[angle];
    y2 = y1 + (USERANGE>>FRACBITS)*finesine[angle];
	
    P_PathTraverse ( x1, y1, x2, y2, PT_ADDLINES, PTR_UseTraverse );
}


void Bot_CheckForProblems(int bot)  
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
	case FOLLOWING_MAIN_PATH:
	//case USING_OBJECT:  // NEW
	// TODO: any other states?
	// NOTE: do not add FOLLOWING_CLOSE here

		curr_node = actor->current_node;
			
		if (curr_node > prev_node)
		{
			actor->previous_node = curr_node;
			return;
		}
			
		if (curr_node == prev_node)
		//if (!P_CheckSight(players[bot].mo, MAIN_PLAYER.mo) )  // UNTESTED
		{
			Bot_Chat(bot, BOTCHATSTUCK);
			Bot_SetState(TRYING_TO_GET_UNSTUCK, bot);
		}
					
		break;

	case TRYING_TO_GET_UNSTUCK:

		curr_node = actor->potential_node;  

		if (curr_node
			&& IsNodeWalkable(actor))  // NEW July 24!!!
		//if (!P_CheckSight(players[bot].mo, MAIN_PLAYER.mo) )  // UNTESTED
		{
			actor->current_node = curr_node;
			Bot_SetState(FOLLOWING_MAIN_PATH, bot);
			//Bot_StartFollowingNearestPath(bot);  
			this_Bot.orders = FOLLOW_LEADER;
		}
		//else
		//{
		//	Bot_StartFollowingPlayer(bot);
		//	this_Bot.orders = FOLLOW_LEADER;
		//}

		break;
	}
}


void Bot_Meander(int bot)
{
	static int  timeout;
	
	mobj_t* actor = this_Bot.player->mo;

	if (this_Bot.state != SEARCHING_FOR_NODES
		&& this_Bot.state != TRYING_TO_GET_UNSTUCK) 
		return;

	if (timeout)
	{
		timeout--;
		Bot_Walk(bot);
		Bot_ChooseTarget(bot); 
	}

	if (!timeout)
	{
		int hi, lo, middle;
		hi = 10; // was 15
		lo = 2;  // was 5
		middle = (hi + lo) / 2;
		
		timeout = GetRandomIntegerInRange(lo, hi);  // number of tics to wait until turning again
		
		if (timeout > middle)  
			actor->angle += ANG45;  // 7-20-19: change to 45 degree angle turn
			//actor->angle += ANG90;
		else
			actor->angle -= ANG45;  // 7-20-19: change to 45 degree angle turn
			//actor->angle += ANG180;
	}
}


void Bot_FollowPath(int bot)
{
	mobj_t* actor = players[bot].mo;

	if (main_path.total_nodes < NODE_START_OFFSET)
		return;

	switch (this_Bot.state)
	{
	case FOLLOWING_MAIN_PATH:   // NEED ANOTHER STATE HERE TO ACCOMODATE FOLLOW_CURSOR ORDERS?

		if (this_Bot.orders == FOLLOW_CURSOR)
		{
			if (!P_CheckSight(actor, actor->target))
				Path_Follow(&main_path, actor);
			else
				Bot_SetState(ENGAGING_ENEMY, bot);
		}
		else
		{
			Path_Follow(&main_path, actor);
		}

		break;
	}
}


void Bot_Dodge(int bot)  // WIP
{
	//Bot_LookForMissiles(bot, MEDIUM_SEARCH_RADIUS);
}



//
//  UTILITY FUNCTIONS
//


int IsBot(player_t* player) 
{
	if (!player)
		return false;

	if (realnetgame)
	    return false;

	if (player->bot_number)
		return player->bot_number;
	else
		return false;
}


void Bot_SetState(int new_state, int bot)
{
	//if (this_Bot.prev_state != this_Bot.state)
	//	this_Bot.prev_state = this_Bot.state;

	this_Bot.state = new_state;
}


void Bot_Cursor()
{
	if (!P_AimLineAttack(MAIN_PLAYER.mo, MAIN_PLAYER.mo->angle, MISSILERANGE / 2)
		|| !linetarget->type == MT_PLAYER)
	{
		Marshmallow_ShowBotReadout = false;
		viewingbot = 0; 
	}
	else
	{
		if (!linetarget->player)  // added to prevent crash here on 6-6-19
			return;

		Marshmallow_ShowBotReadout = true;
		viewingbot = linetarget->player->bot_number;

		if (Bots[viewingbot].orders == FOLLOW_CURSOR)
			lastbot = viewingbot;
	}

	if (Bots[lastbot].orders != FOLLOW_CURSOR)
	{
		crispy->crosshair = 0;
		botcursor_on = false;
	}

#if 0
	// well none of this works anyway...
	// ... but we are able to change color on the fly when colorizing health

	if (viewingbot == 1)
		readoutcolor = CR_GRAY;
	if (viewingbot == 2)
		readoutcolor = CR_GOLD;
	if (viewingbot == 3)
		readoutcolor = CR_DARK;

#endif
	readoutcolor = CR_DARK;

//	if (viewingbot)
//	{
		CrispyReplaceColor( ORDERS, readoutcolor, ORDERS);
		CrispyReplaceColor( STATE, readoutcolor, STATE);
		CrispyReplaceColor( CHAT, readoutcolor, CHAT);
		CrispyReplaceColor( HEALTH, readoutcolor, HEALTH);
//	}

}


void SendKeycardToBots()
{
	card_t keycard;
	bot_t bot;

	for ( keycard = 0; keycard < NUMCARDS ; keycard++)
	{
		if (MAIN_PLAYER.cards[keycard])
			for ( bot = 0 ; bot < MAX_BOTS ; bot++ )  // give it to all bots whether they're playing or not; if we add a bot later, he'll have the key(s)
				players[bot].cards[keycard] = true;
	}
}


boolean 
Bot_BlockThingsIterator
( int			x,
  int			y,
  int			bot,
  boolean(*func)(mobj_t*, int) )  // added this extra argument so we can make this bot-specific
{
    mobj_t*		mobj;
	
    if ( x<0
	 || y<0
	 || x>=bmapwidth
	 || y>=bmapheight)
    {
	return true;
    }
    

    for (mobj = blocklinks[y*bmapwidth+x] ;
	 mobj ;
	 mobj = mobj->bnext)
    {
	if (!func( mobj , bot ) )
	    return false;
    }
    return true;
}


boolean Bot_MonsterCheck (mobj_t* thing, int bot)   // rename...
{
	if (IsMonster(thing)
		&& thing->health > 0 )		
	{
		this_Bot.foundmonster = thing;
		
		return false;
	}
	else
		return true;	// true means we didn't find anything.  so intuitive...
}


void Bot_LookForMonsters (int bot, int radius)
{
    int			xl;
    int			xh;
    int			yl;
    int			yh;
    int			bx;
    int			by;
	fixed_t		tryx;
	fixed_t		tryy;
	fixed_t		xspeed[8] = {FRACUNIT,47000,0,-47000,-FRACUNIT,-47000,0,47000};
	fixed_t		yspeed[8] = {0,47000,FRACUNIT,47000,0,-47000,-FRACUNIT,-47000};

	mobj_t*		actor;
	actor = Bots[bot].player->mo;

	if (!actor)
		return;

	tryx =
	    actor->x + actor->info->speed*xspeed[actor->movedir];
	tryy =
	    actor->y + actor->info->speed*yspeed[actor->movedir];

	xl = (tryx - bmaporgx - MAXRADIUS*radius)>>MAPBLOCKSHIFT;  
	xh = (tryx - bmaporgx + MAXRADIUS*radius)>>MAPBLOCKSHIFT;
	yl = (tryy - bmaporgy - MAXRADIUS*radius)>>MAPBLOCKSHIFT;
	yh = (tryy - bmaporgy + MAXRADIUS*radius)>>MAPBLOCKSHIFT;
	
	for (bx=xl ; bx<=xh ; bx++)
	{
	    for (by=yl ; by<=yh ; by++)
	    {
			if (!Bot_BlockThingsIterator(bx, by, bot, Bot_MonsterCheck))  
			{
				if (!P_CheckSight(actor, this_Bot.foundmonster))   // if we can't see the monster we detected, skip it altogether
					continue;  
				else
					Bot_TakeTarget(bot);
			}
		}
   }
}


void Bot_Think (int bot)
{
	player_t* player = &players[bot];

	if (!player || !player->mo)  // NEW because we crashed in here
		return;

	if (this_Bot.msgtimeout)      
		this_Bot.msgtimeout--;

	if (this_Bot.blacklist_timeout)
		this_Bot.blacklist_timeout--;

	if (player->playerstate == PST_DEAD)  // recently moved this up from below the interp stuff below
    {
		Bot_DeathThink (bot);
		return;
    }

	if (!player->mo->subsector->sector->special)
		this_Bot.lava_timeout = BOT_LAVA_TIMEOUT;  // [marshmallow] so it resets when out of a special sector

    player->mo->interp = true;

    player->mo->oldx = player->mo->x;
    player->mo->oldy = player->mo->y;
    player->mo->oldz = player->mo->z;
    player->mo->oldangle = player->mo->angle;
    player->oldviewz = player->viewz;
    player->oldlookdir = player->lookdir;
    player->oldrecoilpitch = player->recoilpitch;

	P_MovePlayer (player);   
    P_CalcHeight (player);   

    if (player->mo->subsector->sector->special)
	P_PlayerInSpecialSector (player);

	//Bot_PushBarrel(bot);  // no longer needed
}


static bot_weapon_t Bot_GetRandomWeapon(int bfg)  // TODO: enum {NOBFG, WITHBFG}; not for use in the function but when we call it everywhere else
{
	bot_weapon_t weapon = GetRandomIntegerInRange(0, 3 + bfg);

	if ( weapon == BOT_SHOTGUN )
	{
		if ( (GetGameType() != DOOM1) )
			weapon = BOT_SUPERSHOTGUN;   // in Doom2 just default to SSG
	}

	return weapon;
}


static bot_weapon_t Bot_GetBestAvailableWeapon(player_t* player)  
{
	//if (player->weaponowned[wp_bfg])   // no BFG for now
	//	return BOT_BFG;

	bot_weapon_t random_weapon;
	int best_weapon = -1;   // since shotgun starts at zero

	if (player->weaponowned[wp_shotgun])
		best_weapon++;

	//if (player->weaponowned[wp_supershotgun])  // bots will just default to ssg in Doom2
	//	best_weapon++;

	if (player->weaponowned[wp_chaingun])
		best_weapon++;

	if (player->weaponowned[wp_missile])
		best_weapon++;

	if (player->weaponowned[wp_plasma])
		best_weapon++;

	random_weapon = GetRandomIntegerInRange(0, best_weapon);

	return random_weapon;
}


#if 0

int game_type;
char* ShowGameMode();
void ChangeGameMode();

// NOTE:  Ditched this menu option for now (previously was "AI Mode")

void ChangeGameMode() 
{
	int i;
	
	game_type = !game_type;

	if (game_type == COOP)
	{
		deathmatch = 0;  // TODO: is this really how to set coop?
		Marshmallow_FriendlyFire = false;
		nomonsters = false;  // this didn't work here

		for (i=BOT_1;i<=BotsInGame;i++)  // TODO: this doesn't seem to help much; they just keep shooting into oblivion when we change game mode
			Bot_SetState(HOLDING_POSITION, i);
	}
	else if (game_type == DM)
	{
		deathmatch = 3;
		Marshmallow_FriendlyFire = true;
		Marshmallow_DropBackpack = false;
		nomonsters = true;  // this didn't work here

		for (i=BOT_1;i<=BotsInGame;i++)  // HACK: this doesn't seem to help anyway; they just keep shooting into oblivion when we change game mode
			Bot_SetState(HOLDING_POSITION, i);
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}

#endif


void Bot_SetWeapon(int bot)
{
	switch (this_Bot.weaponmenu_selection)
	{
	case BOTMENU_PISTOL:
		this_Bot.weapon = BOT_PISTOL;
		break;
	case BOTMENU_SUPERSHOTGUN:
		this_Bot.weapon = BOT_SUPERSHOTGUN;
		break;
	case BOTMENU_SHOTGUN:
		this_Bot.weapon = BOT_SHOTGUN;
		break;
	case BOTMENU_CHAINGUN:
		this_Bot.weapon = BOT_CHAINGUN;
		break;
	case BOTMENU_PLASMA:
		this_Bot.weapon = BOT_PLASMA;
		break;
	case BOTMENU_GREENPLASMA:
		this_Bot.weapon = BOT_GREENPLASMA;
		break;
	case BOTMENU_MISSILE:
		this_Bot.weapon = BOT_MISSILE;
		break;
	case BOTMENU_BFG:
		this_Bot.weapon = BOT_BFG;
		break;
	case BOTMENU_RANDNOBFG:
		this_Bot.weapon = Bot_GetRandomWeapon(0);
		break;
	case BOTMENU_RANDBFG:
		this_Bot.weapon = Bot_GetRandomWeapon(1);
		break;
	case BOTMENU_AUTO:
		this_Bot.weapon = Bot_GetBestAvailableWeapon(&MAIN_PLAYER);
		break;
	}
}


void Bot_DeathThink (int bot)   
{
//    angle_t		angle;
//    angle_t		delta;
	player_t*   player;

	player = &players[bot];

	if (!deathmatch)
		this_Bot.orders = FOLLOW_LEADER;  

	if (deathmatch || Marshmallow_Sandbox)
	{
        this_Bot.weapon = Bot_GetRandomWeapon(0);	// excluding BFG in deathmatch
	}
	else
	{
		Bot_SetWeapon(bot);
	}

	#if 0
    P_MovePsprites (player);

    // fall to the ground
    if (player->viewheight > 6*FRACUNIT)
	player->viewheight -= FRACUNIT;

    if (player->viewheight < 6*FRACUNIT)
	player->viewheight = 6*FRACUNIT;

    player->deltaviewheight = 0;
    onground = (player->mo->z <= player->mo->floorz);
    P_CalcHeight (player);
	
    if (player->attacker && player->attacker != player->mo)
    {
	angle = R_PointToAngle2 (player->mo->x,
				 player->mo->y,
				 player->attacker->x,
				 player->attacker->y);
	
	delta = angle - player->mo->angle;
	
	if (delta < ANG5 || delta > (unsigned)-ANG5)
	{
	    // Looking at killer,
	    //  so fade damage flash down.
	    player->mo->angle = angle;

	    if (player->damagecount)
		player->damagecount--;
	}
	else if (delta < ANG180)
	    player->mo->angle += ANG5;
	else
	    player->mo->angle -= ANG5;
    }
    else if (player->damagecount)
	player->damagecount--;
#endif

	// TODO: might need a respawn delay; at least in deathmatch

	player->playerstate = PST_REBORN;
}


boolean Bot_AreWeCloseToPlayer(int bot)  // maybe make this generic for use beyond the bots
{
	int dist;
	mobj_t* actor;
	mobj_t* target;

	if (!players[bot].mo)
	{
		//if (Marshmallow_DebugMessages)
		//MAIN_PLAYER.message = "NO TARGET! (CLOSE)";

		return false;
	}

	actor = players[bot].mo;
	target = MAIN_PLAYER.mo;

	if (!target)
		return false;

	dist = P_AproxDistance (target->x - actor->x, 
 					target->y - actor->y);

	if (dist 
		&& dist < TOO_CLOSE_FOR_COMFORT 
		&& !gamekeydown[key_speed])				
	{
		//if (Marshmallow_DebugMessages)
		//MAIN_PLAYER.message = "CLOSE";

		return true;   // we are close to player
	}
	else
		return false;  // we are not close to player yet
}


boolean Bot_CheckRange (mobj_t* actor)    // hollow-out the stuff here not relevant to our bots
{
    fixed_t	dist;

	if (!actor)
	return false;
	
    if (! P_CheckSight (actor, actor->target) )
	return false;
	
    if ( actor->flags & MF_JUSTHIT )
    {
	// the target just hit the enemy,
	// so fight back!
	actor->flags &= ~MF_JUSTHIT;
	return true;
    }
	
    if (actor->reactiontime)
	return false;	// do not attack yet
		
    // OPTIMIZE: get this from a global checksight
    dist = P_AproxDistance ( actor->x-actor->target->x,
			     actor->y-actor->target->y) - 48*FRACUNIT;   // was 64
    
    if (!actor->info->meleestate)
	dist -=96*FRACUNIT;	// was 128
	
    dist >>= FRACBITS;

    if (actor->type == MT_VILE)
    {
	if (dist > 14*64)	
	    return false;	// too far away
    }
	

    if (actor->type == MT_UNDEAD)
    {
	if (dist < 196)	
	    return false;	// close for fist attack
	dist >>= 1;
    }
	

    if (actor->type == MT_CYBORG
	|| actor->type == MT_SPIDER
	|| actor->type == MT_SKULL)
    {
	dist >>= 1;
    }
    
    if (dist > 200)
	dist = 200;
		
    if (actor->type == MT_CYBORG && dist > 160)
	dist = 160;

 //   if (P_Random () < dist)  // TODO: do we want randomness here or not?
	//return false;
		
    return true;
}


void TeleportAnimation(fixed_t x, fixed_t y, fixed_t z)
{
	S_StartSound(NULL, sfx_telept);
	P_SpawnMobj(x, y, z, MT_TFOG); 
}


void Bot_RemoveBots()    
{
	int i;

	if (!BotsInGame)
	{
		SHOW_MESSAGE "NO BOTS CURRENTLY IN GAME!";
		return;
	}

	for (i=BOT_1;i<=BotsInGame;i++)
	if (bot_in_game[i])
	{
		playeringame[i] = false;
		bot_in_game[i] = false;
		
		TeleportAnimation(players[i].mo->x, players[i].mo->y, players[i].mo->z);

		P_RemoveMobj(players[i].mo);  // remve the mobj
		//memset (&players[i], 0, sizeof(players[i]));  // don't do this; causes crash in XYMovement()
		memset (&Bots[i], 0, sizeof(Bots[i]));  // NEW
	}
	
	BotsInGame = 0;
	viewingbot = lastbot = 0;
	Marshmallow_BotEnabled = false;

	//Marshmallow_ShowBotReadout = false;
	//Marshmallow_FriendlyFire = true;
	//Marshmallow_SelfDamage = true;

	if (!deathmatch)
	netgame = false;

	SHOW_MESSAGE "REMOVED ALL BOTS!";
}


void PlayRadioNoise()
{
	if ( gamemode == commercial || Marshmallow_WadStealing)  
	     S_StartSound(0, sfx_radio);
	else
	     S_StartSound(0, sfx_tink);
}


extern hu_stext_t botreadout_chat;  // importing this from hud.h

void Bot_Chat(int bot, char* msg)
{
	// TODO: if bot == BOT_1, then addmsgtotext(bot1readout_chat)
					// BOT_2 ... etc

	CrispyReplaceColor( msg, CR_RED, msg);
	
	HUlib_addMessageToSText(&botreadout_chat, DEH_String(CHAT), /*DEH_String(*/msg/*)*/);

	Bots[bot].msgtimeout = BOT_CHATMSG_TIMEOUT; 
}


void AddCmdLineBots()
{
	int p;

	if (already_spawned_bots)
		return;
	else
		already_spawned_bots = true;

	if ((M_CheckParm ("-bots")) )   // If we only get "-bots" with no number, default to two bots
		BotsInGame = 2;

	if ((M_CheckParm ("-bot") && !BotsInGame))
	{
		AddBot();   
	}

	if ( p = (M_CheckParmWithArgs("-bots", 1)) )
    {
		BotsInGame = myargv[p+1][0]-'0';
		
		if (BotsInGame < 1  )   
			BotsInGame = 2;
			
		if (BotsInGame > 3)
			BotsInGame = 3;
    }

	if (BotsInGame)
		Bot_SpawnBots();
}


void AddBot()
{
	if (BotsInGame < MAX_BOTS-1)    
	{
		BotsInGame++;
		Bot_SpawnBots();
	}
	else
	{
		MAIN_PLAYER.message = "GAME IS NOW FULL!";
	}
}


void SquadRegroup()
{
	int bot;
	for (bot = BOT_1; bot < MAX_BOTS; bot++)  
	{
		if (!bot_in_game[bot])
			continue;

		if ( PlayerIsDead() )
			return;
		
		this_Bot.orders = FOLLOW_LEADER;

		this_Bot.turn_delay = NULL;  // turn instantly, although it sort of looks better when they wait to turn

		this_Bot.targetingDelay = 100;
		//this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT;

		if (P_CheckSight(players[bot].mo, MAIN_PLAYER.mo))   
		{
			// Path_GenerateSegment(this_Bot.player->mo);  // WIP
			Bot_StartFollowingPlayer(bot);
		}
		else
		{
			Bot_StartFollowingNearestPath(bot);
		}
	}

    PlayRadioNoise();
	SHOW_MESSAGE DEH_String(REGROUPSQUAD);
}


// VERIFY: should only be called after bot dies, not when he first joins the game
void Bot_Reborn(int bot)
{
	Bot_Chat(bot, BOTCHATRESPAWN);

	Bot_SetState(JUST_SPAWNED, bot);

	Bot_Stop(bot); 
}


void SetBotReadyWeapon(player_t* bot, int new_weapon)
{
    int botnumber = bot->bot_number;

    if (new_weapon)
    {
        bot->readyweapon = new_weapon;
        return;
    }

    switch (Bots[botnumber].weapon)
    {
        case BOT_PISTOL:
            bot->readyweapon = wp_pistol;
            break;
        case BOT_SHOTGUN:
            bot->readyweapon = wp_shotgun;
            break;
        case BOT_SUPERSHOTGUN:
            bot->readyweapon = wp_supershotgun;
            break;
        case BOT_CHAINGUN:
            bot->readyweapon = wp_chaingun;
            break;
        case BOT_MISSILE:
            bot->readyweapon = wp_missile;
            break;
        case BOT_PLASMA:
            bot->readyweapon = wp_plasma;
            break;
        case BOT_BFG:
            bot->readyweapon = wp_bfg;
            break;
    }
}


void CreateBot(int bot)
{
	float skill_modifier;
	
	playeringame[bot] = true;
	bot_in_game[bot] = true;

	Bots[bot].player = &players[bot]; 

	players[bot].bot_number = bot;

	// Set initial bot state/orders; obviously needs some work...
	if (!deathmatch)
	{
		this_Bot.orders = FOLLOW_LEADER; 
	}
	else
	{
		//Bot_SetState(SEARCHING_FOR_NODES, bot);  // didn't work anyway
	}

	switch (gameskill)
	{
	case 0:
		skill_modifier = 0.8;
		break;
	case 1:
		skill_modifier = 0.9;
		break;
	case 2: // medium
		skill_modifier = 1.0;
		break;
	case 3:
		skill_modifier = 1.1;
		break;
	case 4:
		skill_modifier = 1.2;
		break;
	}

	// Set initial bot weapon
	if (deathmatch || Marshmallow_Sandbox)
	{
		this_Bot.weaponmenu_selection = BOTMENU_RANDNOBFG;
		this_Bot.weapon	= Bot_GetRandomWeapon(0);

		SetBotReadyWeapon(Bots[bot].player, NULL);

		// For deathmatch only; set bot skill levels for deathmatch
		switch (bot)
		{
		case BOT_1: // grey bot will be normal skill
			this_Bot.skill = SKILL_NORMAL*skill_modifier;
			break;
		case BOT_2: // brown bot will be easy skill
			this_Bot.skill = SKILL_EASY*skill_modifier;
			break;
		case BOT_3: // red bot will be hard skill
			this_Bot.skill = SKILL_HARD*skill_modifier;
			break;
		}
	}
	else
	{
		this_Bot.weaponmenu_selection = DEFAULT_BOT_WEAPON;

		if (this_Bot.weaponmenu_selection == BOTMENU_AUTO)
			this_Bot.weapon	= Bot_GetBestAvailableWeapon(&MAIN_PLAYER);
		else
			this_Bot.weapon	= Bot_GetRandomWeapon(0);
	}

	// only do this after the game has started
	if (marshmallow_tic > 30)  
		players[bot].playerstate = PST_REBORN;   // this was necessary in order to remove/add bots during the game
}


void Bot_SpawnBots()
{
	int i;

	// if we haven't used any bots yet, turn on all this stuff first
	if (!Marshmallow_BotEnabled) 
	{
		netgame = true;
		Marshmallow_BotEnabled = true;

		if (!deathmatch)   // most of our default coop flags
		{
			if (M_CheckParm("-ff"))
				Marshmallow_FriendlyFire = true;
			else
				Marshmallow_FriendlyFire = false;

			//Marshmallow_SelfDamage = false;
			//Marshmallow_KeepKeys = true;   // removed March 2020
			//Marshmallow_KeepWeapons = true;   // removed March 2020
			//Marshmallow_DropBackpack = true;

			//if ( !deathmatch )  // added March 2019
			//Marshmallow_WeaponsStay = true; 
		}
	}

	// now we can spawn a bot
	for (i=BOT_1; i<=BotsInGame; i++)   // was <=
	{
		if (!bot_in_game[i])   // was playeringame
			CreateBot(i);
	}
}


static void ShowBotHealth()
{
	int color, health;
	char buff[81];

	mobj_t* bot = players[viewingbot].mo;
	
	if (!bot)
	return;

	if (!viewingbot)  // so we're not colorizing text when we don't need to
	return;

	color = CR_GREEN; // was ColorizeHealth(bot);
	health = bot->health;

	sprintf(buff, "%d", health);
	CrispyReplaceColor( buff, color, buff);
		
	HUlib_addMessageToSText(&botreadout_health, DEH_String(HEALTH), DEH_String(buff));  
}


void UpdateBotReadouts()
{

// Bot orders:
	switch (Bots[viewingbot].orders)
	{
		case FOLLOW_LEADER:
			HUlib_addMessageToSText(&botreadout_orders,DEH_String(ORDERS), DEH_String(COVER));   
			break;

		case FOLLOW_CURSOR:
			HUlib_addMessageToSText(&botreadout_orders, DEH_String(ORDERS), DEH_String(TAKEPOINT));   
			break;

		case PATROLLING:
			HUlib_addMessageToSText(&botreadout_orders, DEH_String(ORDERS), DEH_String(PATROL));   
			break;

		case RETURNING_HOME:
			HUlib_addMessageToSText(&botreadout_orders, DEH_String(ORDERS), DEH_String(BACKTRACK));   
			break;

		case HOLD_POSITION:
			HUlib_addMessageToSText(&botreadout_orders, DEH_String(ORDERS), DEH_String(STAY));   
			break;

		default: 
			HUlib_addMessageToSText(&botreadout_orders, DEH_String(ORDERS), DEH_String(NONE));   
			break;
	}


// Bot states:
	switch (Bots[viewingbot].state)
	{
		case FOLLOWING_MAIN_PATH:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(FOLLOWPATH)); 
			break;

		case ENGAGING_ENEMY:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(ATTACK)); 
			break;

		case JUST_SPAWNED:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(WAIT)); 
			break;

		case FOLLOWING_PATH_REVERSE:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String("RETURNING HOME"));   
			break;

		case SEARCHING_FOR_NODES:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(SEARCHING));   
			break;

		case DEATHMATCH_ROAM_PATH:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(ROAM));  
			break;

		case DEATHMATCH_ROAM_REVERSEPATH:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String("BACKTRACKING"));   
			break;

		case TRYING_TO_GET_UNSTUCK:
			HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String("REGROUPING"));   // was "GETTING UNSTUCK"
			break;

		case FOLLOWING_CLOSE:
				HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(FOLLOW));  
			break;

		case USING_OBJECT:
				HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(USING));  
			break;

		default:
				HUlib_addMessageToSText(&botreadout_state, DEH_String(STATE), DEH_String(FOLLOW)); 
	}		

	ShowBotHealth(); 
}


char* ShowBotState(int bot)
{
	switch (Bots[bot].state)
	{
		case FOLLOWING_MAIN_PATH:

			return DEH_String(FOLLOWPATH);

		case HOLDING_POSITION:

			return DEH_String(STAY);

		case ENGAGING_ENEMY:
			
			return DEH_String(ATTACK);

		case JUST_SPAWNED:
			
			return DEH_String(WAIT);

		case FOLLOWING_PATH_REVERSE:
			
			return DEH_String("RETURNING HOME");  // colorize

		case SEARCHING_FOR_NODES:
			
			return DEH_String(SEARCHING);

		case DEATHMATCH_ROAM_PATH:
			
			return DEH_String(ROAM);

		case DEATHMATCH_ROAM_REVERSEPATH:
		
			return DEH_String("BACKTRACKING");

		case TRYING_TO_GET_UNSTUCK:
			 
			return DEH_String("REGROUPING");  // was "GETTING UNSTUCK"

		case FOLLOWING_CLOSE:
				
			return DEH_String(FOLLOW);

		case USING_OBJECT:
				
			return DEH_String(USING);

		default:
				return DEH_String("NO STATE");
	}	
}


void Bot_ExitLevelCleanup()
{
	int i;

	for (i=BOT_1;i<=BotsInGame;i++)
	{
		if (!bot_in_game[i])
			continue;

		Bot_SetState(JUST_SPAWNED, i);

		players[i].mo->current_node = NULL;
		players[i].mo->potential_node = NULL;
		players[i].mo->target = NULL;

		Bots[i].score = 0;
	}

	memset (&main_path, 0, sizeof(main_path)); 
}


void Bot_ResetAll()
{
	int i;
	
	for (i=BOT_1; i<=BotsInGame; i++)
	{
		if (!bot_in_game[i])
			continue;

		if (!deathmatch) 
		{
		Bots[i].weapon = BOT_SHOTGUN;
		Bots[i].orders = FOLLOW_LEADER;
		Bots[i].state = JUST_SPAWNED;
		}
		else
		{
		Bots[i].state = SEARCHING_FOR_NODES;
		}
	}
}


boolean PlayerInSight(int bot)
{
	if (!players[bot].mo
		|| !MAIN_PLAYER.mo
		|| PlayerIsDead() )  
			return false;

	if ( P_CheckSight(MAIN_PLAYER.mo, players[bot].mo) )   // EXPERIMENT:  putting player first here
		return true;
	else
		return false;
}


void Bot_RevertToLastState(int bot)
{
	switch (this_Bot.orders)
	{
	case FOLLOW_CURSOR:

		this_Bot.orders = FOLLOWING_CLOSE; // NEW
		Bot_StartFollowingPlayer(bot);  // NEW
		
		//Bot_AssignTarget(bot);
		
		break;

	case FOLLOW_LEADER:

		if ( PlayerIsDead() )  // NOTE: without this, we get the Ready/Pathfinding infinite loop	
		{
			//if ( Path_CloseToEnd(&main_path, thisBot_mo) || this_Bot.found_deathnode )
			//{

			//Bot_SetState(FOLLOWING_PATH_REVERSE, bot);  // TESTING REVERSE PATH IN COOP
			Bot_WaitForPlayer(bot);  

			break;
			//}
		}

		if ( PlayerInSight(bot) )
		{
			Bot_StartFollowingPlayer(bot);	 // problem here - they often walk right into lava if they spot player
		}
		else
		{
			Bot_StartFollowingNearestPath(bot);  
		}

		break;

	case PATROLLING:

		Bot_StartPatrolling(bot);

		break;

	case HOLD_POSITION:

		Bot_WaitForPlayer(bot);

		break;
	}
}


boolean Bot_CheckTargetingDelay(int bot)
{
	if (this_Bot.targetingDelay)  
	{
		this_Bot.targetingDelay--;
		return true;  // aborting calling function
	}
	else
	{
		this_Bot.targetingDelay = BOT_TARGET_DELAY;
		return false;
	}
}


boolean Bot_CheckTargetTimeout(int bot)
{
	mobj_t* actor;
	actor = players[bot].mo; 

	 if (actor->target->health <= 0) // first we'll check if target is dead
		 return true;

	// intermittently check if target's health has changed; if equal, ditch target
	if (this_Bot.targetingTimeout)  // so bots don't get stuck on a target too long
	{
		this_Bot.targetingTimeout--;

		return false; 
	}
	else 
	{
		if (this_Bot.target_lasthealth == actor->target->health)
		{
			Bot_Chat(bot, "CAN'T HIT TARGET!"); 

			this_Bot.blacklisted_target = actor->target;
			this_Bot.blacklist_timeout = TARGET_BLACKLIST_DELAY;  

			if (!deathmatch) 
				Bot_RevertToLastState(bot);
		}

		this_Bot.targetingDelay = BOT_TARGET_DELAY * 5;  // wait longer when we ditch a target
		this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT; 
		
		return true;  
	}
}


void Bot_MarkDeathNode(int bot)
{
	this_Bot.found_deathnode = true;

	if (debugmode)
		SHOW_MESSAGE "MARKED";
}


boolean AllBotsMarked() 
{
	boolean all_bots_marked = false;
	int		i;

	for (i = BOT_1; i < BotsInGame; i++)
	{
		if (Bots[i].found_deathnode)
		{
			all_bots_marked = true;
		}
		else
		{
			all_bots_marked = false;
			break;
		}
	}

	return all_bots_marked;
}


void Bot_ResetStuckTimeout(int bot)
{	
	this_Bot.stuck_timeout = BOT_STUCK_TIMEOUT;
}


// So the bots aren't spamming P_CheckSight() every single tic
boolean DoSightcheckTimeout(int bot)
{
	if ( this_Bot.sightcheck_timeout )   
	{                                    
		this_Bot.sightcheck_timeout--;

		return true;    // wait to do sight check
	}
	else
	{
		this_Bot.sightcheck_timeout = SIGHTCHECK_TIMEOUT;

		return false;  // do the sight check 
	}
}


void Bot_ResetOrders()
{
	if (BotsInGame)
	{
		bot_t bot;

		for (bot=BOT_1;bot<=BotsInGame;bot++)
		{
			if (bot_in_game[bot])
			{
				if (deathmatch)
					return;

				this_Bot.orders = FOLLOW_LEADER;
				this_Bot.state = FOLLOWING_CLOSE;

				this_Bot.player->attacker = this_Bot.player->victim = this_Bot.player->mo->target = NULL;
			}

		}
	}
}



//
//  ENTRY POINT
//


void DoAllBot_AI()  
{
	if (BotsInGame)    
	{
		int i;
		
		for (i=BOT_1;i<MAX_BOTS;i++)  
		{
			if (!bot_in_game[i] || !players[i].mo) 
				continue;

			if (!deathmatch)
				Bot_AI(i);
			else
				Deathmatch_Bot_AI(i);
		}
	}
}