//////////////////////////////////
//
//  BOT.C:  Cooperative bots
//
//////////////////////////////////

#include "bot.h"


void Bot_AI(int bot)  
{
	Bot_Think(bot);

	Path_SearchForNodes(&main_path, players[bot].mo);   
	
	Bot_CheckForProblems(bot);
	Bot_FollowPath(bot);
	Bot_ChooseTarget(bot); 
	Bot_Attack(bot);		
	Bot_FollowTarget(bot); 
	Bot_UseStuff(bot); 
	Bot_Meander(bot);

	Bot_Cursor();
}


void Bot_ChooseTarget(int bot)  
{
	mobj_t*		actor;  
	actor = this_Bot.player->mo;

	if (marshmallow_tic < 50)   // wait a bit at the beginning of the map to start shooting
		return;

	if (sandbox.design_mode
		|| Bot_CheckTargetingDelay(bot))
	{
		return;
	}

	switch (this_Bot.state)
	{
		case FOLLOWING_CLOSE:
		case FOLLOWING_MAIN_PATH:
		case FOLLOWING_PATH_REVERSE:
		case HOLDING_POSITION:       
		case SEARCHING_FOR_NODES:    
		case AWAITING_TARGET: 

			// target player's attacker or take his current target
			Bot_DefendPlayer(bot);	
	
			// if the above gets us a target, we're outta here
			if (this_Bot.state == ENGAGING_ENEMY)  
				return;

			// if we didn't already get a target, make sure we defend ourself
			Bot_DefendSelf(bot);

		default:
			return;
	}
}


void Bot_FollowTarget(int bot)    
{
	mobj_t* actor = players[bot].mo;

	switch (this_Bot.state)
	{
	case SEARCHING_FOR_NODES:
	case ENGAGING_ENEMY:
		break;

	case HOLDING_POSITION:
	
		Bot_Stop(bot);

		if (this_Bot.orders == FOLLOWING_CLOSE 
			&& PlayerInSight(bot))
		{
			Bot_StartFollowingPlayer(bot);   
				
			if ( AllBotsMarked() )
				Path_ClearDeathNode();
		}

		break;

	case USING_OBJECT:

		Bot_Stop(bot);

		if ( this_Bot.usenode_timer )
		{
			this_Bot.usenode_timer--;

			if ( this_Bot.usenode_timer == 0 )
			{
				//thisBot_mo->current_node++;
				Bot_SetState(FOLLOWING_MAIN_PATH, bot);
				//Bot_StartFollowingNearestPath(bot);
			}
		}

		break;

	case FOLLOWING_MAIN_PATH:
	case DEATHMATCH_ROAM_PATH:

		if ( main_path.use_node[ actor->current_node/*-1*/ ]     // TODO: try moving all of this up to USING_OBJECT
			&& !this_Bot.usenode_timer
			&& specialnodes) 
		{
			Bot_Stop(bot);
			
			// NOTE: did the angle change in here give us a G_CheckSpot crash in MAP06????
			actor->angle = main_path.angle[ actor->current_node/*-1*/ ];

			//Bot_Run(bot); // move up
			//Bot_Run(bot); // move up
			//Bot_Run(bot); // move up
			// TODO Bot_NudgeForward(bot, amount); ... amount just repeats Bot_Run() or Bot_Walk()

			usedelay = 0;  // TODO:  bot-specific usedelay
			//this_Bot.waiting_for_door = true;
			this_Bot.usenode_timer = 10;  // was 15
			Bot_SetState(USING_OBJECT, bot);
			SHOW_MESSAGE "USING SOMETHING!";

			break;
		}

		if (!Path_CloseToEnd(&main_path, actor) )
		{
			if ( main_path.sprint_node[ actor->current_node/*+1*/ ] 
				&& specialnodes)  // Find a "sprint node"
			{
				Bot_Stop(bot);
				Bot_Sprint(bot);
			}
			else
				Bot_Walk(bot); 
		}
		else
		{   
			//if ( !PlayerInSight(bot) )  // NEW testing.....
			//	Bot_SetState(HOLDING_POSITION, bot);  // testing........

			Bot_Stop(bot); 

			Bot_FacePlayer(actor);
			Bot_SetState(FOLLOWING_CLOSE, bot);		
		}

		break;

	case FOLLOWING_CLOSE:

		if ( PlayerInSight(bot) && PlayerIsDead() )   
			break;
		
		//if ( DoSightcheckTimeout(bot) )   
		//	break;
		
		if ( !PlayerInSight(bot) )  
		{
			Bot_Chat(bot, "LOST YOU!");
			Bot_StartFollowingNearestPath(bot);
		}
		else
		{
			Bot_FacePlayer(actor);  // problem?

			if (!Bot_AreWeCloseToPlayer(bot))  
				Bot_Walk(bot);
			else
				Bot_Stop(bot);
		}

		break;

	case JUST_SPAWNED:

		actor->current_node = NODE_CHAIN_START; 
		 
		if ( PlayerInSight(bot) )
		{
			
			//if (default_bot_speed == BOTS_CAREFUL)  // NEW march 2020 testing........
			//{
			//	Bot_StartFollowingNearestPath(bot);
			//	break;
			//}

			Bot_StartFollowingPlayer(bot);
		}
		else
		{
			Bot_StartFollowingNearestPath(bot);
		}
			
		break;  
	}	
}


void Bot_Attack (int bot)
{
	mobj_t* actor;
	mobj_t* target;

	if (!players[bot].mo)
		return;				
	
	actor = players[bot].mo; 
	target = actor->target;

	if (this_Bot.state != ENGAGING_ENEMY
		|| players[bot].playerstate != PST_LIVE)  
		return;

	// In Sandbox mode, don't shoot anything if not in battle
	if (sandbox.design_mode   
		&& thisBot_mo->target)
	{
		thisBot_mo->target = NULL;
		Bot_SetState(FOLLOW_LEADER, bot);
		return;
	}

	// If we kill something, revert to whatever we were doing before taking a target  // TODO:  roll into DidWeKillTarget()
	if (actor->target->health <= 0)							
	{
		Bot_Chat(bot, "GOT ONE!");
		Bot_RevertToLastState(bot);
		Bots[bot].targetingDelay = BOT_TARGET_DELAY;  // was NULL

		return;
	}

	if ( Bot_CheckTargetTimeout(bot) )
		return;

	Bot_Stop(bot);  

	// Target isn't dead yet so keep shooting it			
	if (target->health > 0)
    {
		A_FaceTarget (actor);  
		Bot_Shoot(bot);

		// If we lost sight of target, disengage
		if (!P_CheckSight(actor, target))   
		{
			Bot_Chat(bot, "LOST TARGET!");

			this_Bot.turn_delay = BOT_TURN_DELAY;

			Bot_RevertToLastState(bot);
		}
	}	
	else
	{
		Bot_RevertToLastState(bot);
	}
}


void Bot_DefendSelf(int bot)
{
	mobj_t*		actor;  
	actor = this_Bot.player->mo;

	if (this_Bot.player->attacker										
		&& this_Bot.player->attacker->health > 0
		&& this_Bot.player->attacker->attacking_player == 1
		&& P_CheckSight(actor, this_Bot.player->attacker))  
	{
		if (this_Bot.player->attacker->type == MT_PLAYER/* && !deathmatch*/) // removed 2-17-2020
		{
			Bot_LookForMonsters(bot, MEDIUM_SEARCH_RADIUS);  
			return;  // don't retaliate against other players when FF is on
		}

		actor->target = this_Bot.player->attacker;
		A_FaceTarget(actor);

		Bot_SetState(ENGAGING_ENEMY, bot);
		this_Bot.target_lasthealth = actor->target->health;
		this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT; 
		Bot_Chat(bot, BOTCHATFIRE);
	}
	else
	{
		Bot_LookForMonsters(bot, MEDIUM_SEARCH_RADIUS);   // if none of the above methods get us a target, start searching the area for targets
	}
}


void Bot_DefendPlayer(int bot)
{
	mobj_t*	actor = this_Bot.player->mo;

	// Defend player by targeting his attacker											
	if (MAIN_PLAYER.attacker
		&& MAIN_PLAYER.attacker->health > 0
		&& MAIN_PLAYER.attacker->attacking_player == 1
			&& P_CheckSight(actor, MAIN_PLAYER.attacker) ) 
	{

		if (MAIN_PLAYER.attacker->type == MT_PLAYER && !deathmatch)
		{
			Bot_LookForMonsters(bot, MEDIUM_SEARCH_RADIUS);  
			return;  // don't retaliate against other players when FF is on
		}


		if (MAIN_PLAYER.attacker == this_Bot.blacklisted_target
			&& this_Bot.blacklist_timeout)
				return;

		actor->target = MAIN_PLAYER.attacker;
		A_FaceTarget(actor);

		Bot_SetState(ENGAGING_ENEMY, bot);
		this_Bot.target_lasthealth = actor->target->health;
		this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT; 
		Bot_Chat(bot, BOTCHATCOVER);

		return;
	} 
	
	// Or just take player's current target			
	if (MAIN_PLAYER.mo->target													
		&& MAIN_PLAYER.mo->target->health > 0
		&& MAIN_PLAYER.mo->target->attacking_player == 1
		&& P_CheckSight(actor, MAIN_PLAYER.mo->target) ) 
	{
		if (MAIN_PLAYER.mo->target == this_Bot.blacklisted_target
			&& this_Bot.blacklist_timeout)
				return;

		actor->target = MAIN_PLAYER.mo->target;
		A_FaceTarget(actor);

		Bot_SetState(ENGAGING_ENEMY, bot);
		this_Bot.target_lasthealth = actor->target->health;
		this_Bot.targetingTimeout = DEFAULT_BOT_TARGET_TIMEOUT; 
		Bot_Chat(bot, BOTCHATGETHIM);
	}
}