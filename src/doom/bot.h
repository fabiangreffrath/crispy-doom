#ifndef __MARSHMALLOW_BOT__
#define __MARSHMALLOW_BOT__

#include "marshmallow.h"

// Bot globals		
boolean bot_in_game[MAXPLAYERS];
boolean already_spawned_bots;
boolean ShowBotStates;
int BotsInGame;
int usedelay;	
int default_bot_speed;
int dm_frags[MAXPLAYERS][MAXPLAYERS];

// Some useful macros
#define this_Bot	 Bots[bot]
#define thisBot_mo   Bots[bot].player->mo 

typedef enum {

	TOO_CLOSE_FOR_COMFORT = 18000000,
	BOT_USE_DELAY = 55,  // used to be 30; increased because of some very tall doors in TNT.WAD
	BOT_TARGET_DELAY = 12,  
	TARGET_BLACKLIST_DELAY = 250, 
	DEFAULT_BOT_TARGET_TIMEOUT = 100, 
	BOT_STUCK_TIMEOUT = 100,   // was 150 
	BOT_LAVA_TIMEOUT = 550,  // this is how long bots stand in lava before suicide/respawn
	BOT_TURN_DELAY = 25, 
	MEDIUM_SEARCH_RADIUS = 20,  
	BOT_CHATMSG_TIMEOUT = 100,  
	DEFAULT_RESPAWN_DELAY = 50,
	SIGHTCHECK_TIMEOUT = 20,  // implemented but not in use
	FIRST_BOT_WEAPON = 2,  // 2 = BOT_MISSILE
	DEFAULT_BOT_WEAPON = 10, // 10 = BOTMENU_AUTO
    WEAPON_SWITCH_DELAY = 50 // tics

} bot_defaults_t;


typedef enum {

	BOT_FIRINGDELAY_MISSILE = 20,
	BOT_FIRINGDELAY_PISTOL = 15,
	BOT_FIRINGDELAY_SHOTGUN = 30,
	BOT_FIRINGDELAY_SUPERSHOTGUN = 40,
	BOT_FIRINGDELAY_CHAINGUN = 3,
	BOT_FIRINGDELAY_PLASMA = 4,
	BOT_FIRINGDELAY_BFG = 50

} bot_weapontimers_t;


void Bot_DefendPlayer(int bot);  
void Bot_DefendSelf(int bot); 
void Bot_RevertToLastState(int bot);  
void Bot_SetState(int new_state, int bot); 
void Bot_Shoot(int bot);
void Bot_AI(int bot);
void Bot_FollowPath(int bot);
void SendKeycardToBots();
void Bot_SpawnBots();
void Bot_Reborn(int bot);
void Bot_UseStuff(int bot);
void Bot_Dodge(int bot);
void Bot_Attack (int bot);
void Bot_DeathThink (int bot);
void Bot_SetWeapon(int bot);
void Bot_Think(int bot);
void Bot_HoldPosition();
void Bot_PlayRadioNoise();
void CreateBot(int bot);  
void Bot_GiveOrders();
void Bot_WaitForPlayer(int bot);
void Bot_TakeTarget(int bot);
void Bot_LookForMonsters (int bot, int radius);
void Bot_FollowTarget(int bot); 
void Bot_ChooseTarget(int bot);
void Bot_Chat(int bot, char* msg);
void Bot_StartFollowingPlayer(int bot); 
void Bot_StartPatrolling(int bot);
void Bot_AssignTarget(int bot);
void Bot_AssignTargetToAll();
void Bot_Meander(int bot);
void Bot_Cursor();
void Path_TrackPlayerPosition(navpath_t* path, mobj_t* actor);
void Path_PlayerNodes();
void Bot_ResetStuckTimeout(int bot);
void Bot_Run(int bot);
void Bot_Walk(int bot);
void Bot_Sprint(int bot);
void Bot_Stop(int bot);
void Bot_FacePlayer(mobj_t* actor);
void Bot_MarkDeathNode(int bot);
void Bot_CycleOrders();
void UpdateBotReadouts();
void Bot_ResetOrders();
void Deathmatch_Bot_ChooseTarget(int bot); 
void Deathmatch_Bot_AI(int bot); 
void Deathmatch_Bot_Run(int bot); 
void Deathmatch_Bot_Attack(int bot);
int Deathmatch_PlayerInSight(int bot); 
int Deathmatch_SearchForNodes(navpath_t* path, mobj_t* actor);
int Deathmatch_SearchForNodesFromEnd(navpath_t* path, mobj_t* actor);
boolean AllBotsMarked();
boolean Bot_CheckTargetingDelay(int bot);
boolean PlayerInSight(int bot);  
boolean Bot_AreWeCloseToPlayer(int bot);
boolean Bot_MonsterCheck (mobj_t* thing, int bot);
boolean Bot_CheckRange (mobj_t* actor);
boolean Bot_BlockThingsIterator ( int x, int y, int bot, boolean(*func)(mobj_t*, int) );
boolean Bot_CheckTargetTimeout(int bot);
boolean DoSightcheckTimeout(int bot);
navpath_t* Bot_StartFollowingNearestPath(int bot);
void CorpseCleanup();
void DroppedItemCleanup();


typedef enum {

	NO_BOTS,
	BOT_1,
	BOT_2,
	BOT_3,
	MAX_BOTS,

} bot_t;


typedef enum {

	JUST_SPAWNED,
	FOLLOWING_CLOSE,
	FOLLOWING_MAIN_PATH,
	FOLLOWING_PATH_REVERSE,
	//FOLLOWING_MY_PATH,  
	//FOLLOWING_MY_PATH_REVERSE,
	HOLDING_POSITION,
	USING_OBJECT,
	AWAITING_TARGET,
	ENGAGING_ENEMY,
	SEARCHING_FOR_NODES,
	DEATHMATCH_ROAM_PATH,
	DEATHMATCH_ROAM_REVERSEPATH,
	TRYING_TO_GET_UNSTUCK,

	NUMBER_OF_BOTSTATES,

} bot_state_t;


typedef enum {

	NO_ORDERS,     // NOTE: don't use the zero position because of our ++ and -- operations
	FOLLOW_LEADER,
	HOLD_POSITION,
	PATROLLING,
	
	FRAGFEST,  // NEW for DM
	
	MAX_ORDERS,
	
	// unused
	FOLLOW_CURSOR,
	RETURNING_HOME,

} bot_orders_t;


typedef enum {

	BOT_SHOTGUN,
	BOT_CHAINGUN,
	BOT_MISSILE,
	BOT_PLASMA,
	BOT_BFG,

	// Weapons excluded from random selection:
	
	BOT_GREENPLASMA,  
	BOT_PISTOL, 
	BOT_SUPERSHOTGUN,  

	MAX_BOTWEAPONS,

} bot_weapon_t;  


typedef enum {

	BOTMENU_PISTOL,
	BOTMENU_SHOTGUN,
	BOTMENU_SUPERSHOTGUN,
	BOTMENU_CHAINGUN,
	BOTMENU_PLASMA,			
	BOTMENU_GREENPLASMA,
	BOTMENU_MISSILE,
	BOTMENU_BFG,
	BOTMENU_RANDNOBFG, 
	BOTMENU_RANDBFG, 
	BOTMENU_AUTO,   

	BOTMENU_END

} bot_weaponmenu_t;


typedef enum {

	SKILL_EASY = 56,  // was 50
	SKILL_NORMAL = 60, // was 55
	SKILL_HARD = 64  // was 60

} bot_skill_t;


typedef struct marshmallow_bot_s {

	player_t*		player;  

	bot_state_t		state;

	bot_skill_t		skill;

	navpath_t		attack_path;
	int				cached_node;  

	bot_orders_t	orders; 
	bot_weapon_t	weapon;

	boolean			waiting_for_door;

	hu_stext_t		chatmsg;
	int				msgtimeout;

	int				targetingDelay;
	int				targetingTimeout;

	mobj_t*			blacklisted_target; 
	int				blacklist_timeout;

	int				firingDelay;	
	int				attentionTimeout;

	int				current_waypoint;		
	int				cached_waypoint;		

	int				target_lasthealth;
		
	int				strafe_delay;
	boolean			strafe_direction;

	mobj_t*			foundmonster;
	mobj_t*			foundmissile;

	int				score;   

	int				weaponmenu_selection;		
	int				stuck_timeout;  
	int				lava_timeout;  

	int				sightcheck_timeout;

	int				turn_delay; 

	int				found_deathnode;  

	int				dodge_timer;

	int				usenode_timer;

	int             weapon_switch_delay;

} marshmallow_bot_t;

marshmallow_bot_t Bots[MAXPLAYERS];  // NOTE: slot 0 will remain unused so that bots 1,2,3 line up with playeringame[] array

navpath_t attack_path[MAXPLAYERS];

#endif