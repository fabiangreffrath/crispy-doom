
// these are useful if we don't need to factor-in wad stealing (which is what GetGameType() is for)
#define PLAYING_DOOM1 (gamemode != commercial)
#define PLAYING_DOOM2 (gamemode == commercial)

// for GetGameType() return value
enum { DOOM1, DOOM2, BOTH, WHICH_GAME }; 

enum { OFF, ON };
enum { SINGLEPLAYER, COOP, DEATHMATCH , SANDBOX };  
enum { HIDE, SHOW };
enum { WEAPON, AMMO, MEDKIT, ARMOR, POWERUP };
enum { BOTS_CAREFUL, BOTS_WALK, BOTS_RUN };
typedef enum {BACKWARD,	FORWARD} direction_t;
typedef int playerindex_t;

#define MF_TREASURE 0x90000000 // was MF_AMBUSH
#define DEATHMATCH_ITEM (mthing->options & 16)
#define DEFAULT_TARGET_TIMEOUT 175   // 5 seconds
#define DEFAULT_ATTACKER_TIMEOUT 30 // 875 is 25 seconds
#define BARREL_PUSH_THRUST 90000

#define FIRST_MENU_ITEM 1

#define DOOMLABEL  "       DOOM"
#define DOOM2LABEL "       DOOM 2"
#define BOTHLABEL  "       BOTH"

#define WALK_SPEED 0

#define DEMONSPHERE_HEALTH_PERCENTAGE 0.01   // at 0.01%, the Hell Knight gave us 5 health

#define PORTABLE_MEDKIT 666

#define SIGHT_TEST MT_SMOKE

#define MINUTE 2100  // 2100 tics = 1 minute

// Shortcuts for checking certain maps
// NOTE: do checks for Doom/Doom2 before these are called
#define MAP01 (gamemap == 1)
#define MAP07 (gamemap == 7)
#define MAP15 (gamemap == 15)
#define MAP32 (gamemap == 32)
#define E1M1 (gamemap == 1 && gameepisode == 1)
#define E1M8 (gameepisode == 1 && gamemap == 8)
#define E2M8 (gameepisode == 2 && gamemap == 8)
#define E3M8 (gameepisode == 3 && gamemap == 8)

#define PLAYER_INVISIBLE (Marshmallow_TrueInvisibility && player->powers[pw_invisibility])

#define MENUKEY_PREVIOUS (gamekeydown[key_previtem] || gamekeydown[key_up])
#define MENUKEY_NEXT (gamekeydown[key_nextitem] || gamekeydown[key_down])
#define MENUKEY_SELECT (gamekeydown[key_useitem] || gamekeydown[key_enter])
#define MENUKEY_BACKTRACK (gamekeydown[key_escape])
#define MENUKEY_ARROWKEYS ( gamekeydown[key_left] || gamekeydown[key_right] )
#define MENUKEY_LEFTARROW (gamekeydown[key_left])
#define MENUKEY_RIGHTARROW (gamekeydown[key_right])

// My own labels for awfully-named vanilla doom objects
#define MARSHMALLOW_BACKPACK			MT_MISC24   // TODO:  remove MARSHMALLOW_ prefix
#define MARSHMALLOW_SMALL_FLOORLAMP	MT_MISC31   // TODO:  remove MARSHMALLOW_ prefix
#define DOOM1_POOL_OF_BLOOD		MT_MISC71
#define MT_RPG					MT_MISC27
#define MT_BFG9000				MT_MISC25
#define MT_PLASMA_RIFLE			MT_MISC28
#define MT_BOX_OF_BULLETS		MT_MISC17
#define MT_BOX_OF_SHELLS		MT_MISC23
#define MT_BOX_OF_ROCKETS		MT_MISC19 
#define MT_BOX_OF_CELLS			MT_MISC21 
#define MT_HEALTH_BONUS			MT_MISC2
#define MT_ARMOR_BONUS			MT_MISC3
#define MT_MEDKIT				MT_MISC11
#define MT_STIMPACK				MT_MISC10
#define MT_SHELLS				MT_MISC22
#define MT_ROCKETAMMO			MT_MISC18
#define MT_CELL					MT_MISC20
#define MT_CHAINSAW				MT_MISC26
#define MT_BLUEKEY				MT_MISC4
#define MT_REDKEY				MT_MISC5
#define MT_YELLOWKEY			MT_MISC6
#define MT_YELLOWSKULLKEY		MT_MISC7
#define MT_REDSKULLKEY			MT_MISC8
#define MT_BLUESKULLKEY			MT_MISC9
#define MT_GREENARMOR			MT_MISC0
#define MT_BLUEARMOR			MT_MISC1
#define MT_BERSERK				MT_MISC13
#define MT_SOULSPHERE			MT_MISC12
#define MT_INVISIBILITY			MT_INS
#define MT_DEMONSPHERE			MT_INS
#define MT_GOGGLES				MT_MISC16 

// Bot movement signals
#define BOT_WALK_SPEED 0
#define BOT_RUN_SPEED 1
#define BOT_STOP_MOVING 0
#define BOT_STRAFE_LEFT 0
#define BOT_STRAFE_RIGHT 1

// Treasure items
#define YELLOW_SKULL MT_MISC7
#define RED_SKULL MT_MISC8 
#define BLUE_SKULL MT_MISC9
#define CANDLE MT_MISC49

// Object dropping   ( ... are we still using this junk?)
#define MARSHMALLOW_DROPOBSTACLE_OFFSET_X 4000000
#define MARSHMALLOW_DROPOBSTACLE_OFFSET_Y 4000000
#define MARSHMALLOW_ITEMDROP_OFFSET 2000000
#define OBJECT_DROP_DELAY 25

// For red screen flash on berserk pickup
#define BERSERK_REDSCREEN_OFF 0		
#define BERSERK_REDSCREEN_SHORT 2
#define BERSERK_REDSCREEN_VANILLA 12
#define NEW_DAMAGE_REDSCREEN_INTENSITY (cnt+7)>>6;		// vanilla was >>3

// Ammo amounts for our new backpack drop on death
#define MARSHMALLOW_BACKPACK_BULLETS 10
#define MARSHMALLOW_BACKPACK_SHELLS 4
#define MARSHMALLOW_BACKPACK_MISSILES 1
#define MARSHMALLOW_BACKPACK_CELLS 20
#define MARSHMALLOW_BACKPACK_HEALTHPOINTS 5
#define MARSHMALLOW_BACKPACK_ARMORPOINTS 5
#define MARSHMALLOW_OBSTACLE_ARMOR_COST 10
#define MARSHMALLOW_BARREL_MISSILE_COST 2

// Mirror damage penalties based on ammo type
#define MARSHMALLOW_FF_PENALTY_BULLETS 10
#define MARSHMALLOW_FF_PENALTY_SHOTGUN 25
#define MARSHMALLOW_FF_PENALTY_SUPERSHOTGUN 35
#define MARSHMALLOW_FF_PENALTY_MISSILES 50
#define MARSHMALLOW_FF_PENALTY_PLASMA 20
#define MARSHMALLOW_FF_PENALTY_BFG 70
#define MARSHMALLOW_FF_PENALTY_MELEE 15

// Shortcuts for referencing the players array
#define MAIN_PLAYER players[consoleplayer]
#define SHOW_MESSAGE players[consoleplayer].message = 
#define SHOW_CENTERED_MESSAGE players[consoleplayer].centermessage = 
#define MARSHMALLOW_SIGNAL players[i].cmd.chatchar  // unused?

// Sprint feature
#define SPRINT_SPEED MAIN_PLAYER.cmd.forwardmove*2048 + 100000
#define DEFAULT_SPRINT_TIMEOUT 50
#define DEFAULT_SPRINT_RECHARGE 150

// New in-game cheats 
#define MARSHMALLOW_CHEAT_PHYSICS "insane"
#define MARSHMALLOW_CHEAT_FLIPLEVELS "idflip"
#define MARSHMALLOW_CHEAT_BOTMOVE "move"
#define MARSHMALLOW_CHEAT_BOTWAIT "hold"
#define MARSHMALLOW_CHEAT_FRIENDLYFIRE "friendlyfire"
#define MARSHMALLOW_CHEAT_CHECKSKILL "skill"
#define MARSHMALLOW_CHEAT_CHANGESKILL1 "skillone"  
#define MARSHMALLOW_CHEAT_CHANGESKILL2 "skilltwo"   // this 't' breaks it in netgame because it triggers chat
#define MARSHMALLOW_CHEAT_CHANGESKILL3 "skillthree"  // this 't' breaks it in netgame because it triggers chat
#define MARSHMALLOW_CHEAT_CHANGESKILL4 "skillfour"
#define MARSHMALLOW_CHEAT_CHANGESKILL5 "skillfive"
#define MARSHMALLOW_CHEAT_CHANGESKILL6 "skillsix"
#define MARSHMALLOW_CHEAT_ENDMAPNOW "endmap" 
#define MARSHMALLOW_CHEAT_RESTARTMAP "redomap" 
#define MARSHMALLOW_CHEAT_KILLMENOW "killmenow"
#define MARSHMALLOW_CHEAT_AUTOUSE "quakeuse"
#define MARSHMALLOW_CHEAT_GAMEMODE "gamemode" 
#define MARSHMALLOW_CHEAT_SHOWINFO "showinfo" 
#define MARSHMALLOW_CHEAT_EKG "ludicrous"
#define MARSHMALLOW_CHEAT_ALLITEMS "giveall"
#define MARSHMALLOW_CHEAT_DIPSTICK "chojin"
#define MARSHMALLOW_CHEAT_FASTMONSTERS "fastmode"
#define MARSHMALLOW_CHEAT_NORESPAWN "norespawn"
#define MARSHMALLOW_CHEAT_NEXTTRACK "next"
// TODO: "lastmap"

#define WAYPOINT_OBJECT_FOR_DOOM1 DOOM1_POOL_OF_BLOOD   // back to the small candle because the pool of blood wasn't in Doom1   
//#define WAYPOINT_OBJECT_FOR_DOOM2 MT_WAYPOINT  // currently a custom "entity" that uses the pool of blood sprite

// for an extra line of text on the HUD
#define		MARSHMALLOW_HU_PREVMSG_Y	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +1))  // input line was +1, I made it +2 UPDATE: this number might be pixels or something; NOT lines NEW UPDATE: looks like +8 gave us exactly one line
#define		MARSHMALLOW_HU_INPUTY	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +8))  // input line was +1, I made it +2 UPDATE: this number might be pixels or something; NOT lines NEW UPDATE: looks like +8 gave us exactly one line
#define		MARSHMALLOW_HU_MSGTIMEOUT	(4*TICRATE)

// enemy balancing
#define VILE_Z_SCOPE_LIMIT 100*FRACUNIT
#define NERFED_CACO_SPAWNHEALTH 100   // default is 400
#define NERFED_LOSTSOUL_SPAWNHEALTH 25  // default is 100
#define NERFED_SKELSHOT_SPEED 5*FRACUNIT; // vanilla is 10
#define NERFED_VILE_DAMAGE 50			 // default is 70
#define VANILLA_VILE_DAMAGE 70			 
#define NERFED_DEMON_DAMAGE 2            // default was 4
#define BOOSTED_BRUISERSHOT_SPEED 20*FRACUNIT;  // vanilla is 15	 NOTE:  clean up the semicolons in these macros!
#define BOOSTED_TROOPSHOT_SPEED 15*FRACUNIT;   // vanilla is 10
#define BOOSTED_CACOSHOT_SPEED 12*FRACUNIT;   // vanilla is 10
#define BOOSTED_ZOMBIEMAN_SPEED 12 // vanilla is 8
#define NEW_ZOMBIEMAN_REACTIONTIME 4 // vanilla was 8 
#define NEW_DEMON_REACTIONTIME 4 // vanilla was 8

int chaingunguy_attack_sound;  // TODO: move out of this file
int vile_damage;  // TODO: move out of this file

//  event triggers which are sent covertly over the chat system
enum {
	MARSHMALLOW_SOMEONE_ABORTED_MAP,
	MARSHMALLOW_SOMEONE_KILLED_THEMSELF,
	MARSHMALLOW_BACKPACK_WAS_DROPPED,
	MARSHMALLOW_BARREL_WAS_DROPPED,
	MARSHMALLOW_OBJECT_WAS_DROPPED,
	MARSHMALLOW_SPAWN_BOTS,
	PLAYER0_NEXTOBJ,
	PLAYER0_PREVOBJ,
	PLAYER1_NEXTOBJ,
	PLAYER1_PREVOBJ,
	PLAYER2_NEXTOBJ,
	PLAYER2_PREVOBJ,
	PLAYER3_NEXTOBJ,
	PLAYER3_PREVOBJ,
	START_BATTLE,
};


// ASCII values for our new netgame event signals
#define MARSHMALLOW_CHATCHAR_PLAYER0_NEXTOBJECT  0xE8         //232
#define MARSHMALLOW_CHATCHAR_PLAYER0_PREVOBJECT  0xE9         //233
#define MARSHMALLOW_CHATCHAR_PLAYER1_NEXTOBJECT  0xEA         //234
#define MARSHMALLOW_CHATCHAR_PLAYER1_PREVOBJECT  0xEB         //235
#define MARSHMALLOW_CHATCHAR_PLAYER2_NEXTOBJECT  0xEC         //236
#define MARSHMALLOW_CHATCHAR_PLAYER2_PREVOBJECT  0xED         //237
#define MARSHMALLOW_CHATCHAR_PLAYER3_NEXTOBJECT  0xEE         //238
#define MARSHMALLOW_CHATCHAR_PLAYER3_PREVOBJECT  0xEF         //239

#define MARSHMALLOW_CHATCHAR_TOQUIT	0xF0					//240	360	F0	11110000	ð	&#240;	&eth;	Latin small letter eth
#define MARSHMALLOW_CHATCHAR_TOKILL		0xF1				//241	361	F1	11110001	ñ	&#241;	&ntilde;	Latin small letter n with tilde        
#define MARSHMALLOW_CHATCHAR_TO_DROPGIFT		0xF2			//242	362	F2	11110010	ò	&#242;	&ograve;	Latin small letter o with grave   
#define MARSHMALLOW_CHATCHAR_TO_DROPBARREL	0xF3			//243	363	F3	11110011	ó	&#243;	&oacute;	Latin small letter o with acute 
#define MARSHMALLOW_CHATCHAR_TO_DROPMONSTER	0xF4			//244	364	F4	11110100	ô	&#244;	&ocirc;	Latin small letter o with circumflex 
#define MARSHMALLOW_CHATCHAR_TO_STARTBATTLE 0xF5					//245	365 F5  11110101    õ   &#245;  &otilde; Latin small letter o with tilde
#define MARSHMALLOW_CHATCHAR_TOSEND_CHAINSAW	0xF6			//246	366	F6	11110110	ö	&#246;	&ouml;	Latin small letter o with diaeresis
#define MARSHMALLOW_CHATCHAR_TOSEND_SHOTGUN	0xF7			//247	367	F7	11110111	÷	&#247;	&divide;	Division sign
#define MARSHMALLOW_CHATCHAR_TOSEND_SSG  0xF8					//248	370	F8	11111000	ø	&#248;	&oslash;	Latin small letter o with slash
#define MARSHMALLOW_CHATCHAR_TOSEND_CHAINGUN	0xF9			//249	371	F9	11111001	ù	&#249;	&ugrave;	Latin small letter u with grave
#define MARSHMALLOW_CHATCHAR_TOSEND_RPG	0xFA				//250	372	FA	11111010	ú	&#250;	&uacute;	Latin small letter u with acute
#define MARSHMALLOW_CHATCHAR_TOSEND_PLASMA	0xFB			//251	373	FB	11111011	û	&#251;	&ucirc;	Latin small letter u with circumflex
#define MARSHMALLOW_CHATCHAR_TOSEND_BFG	0xFC				//252	374	FC	11111100	ü	&#252;	&uuml;	Latin small letter u with diaeresis
#define MARSHMALLOW_CHATCHAR_TOSEND_BLUEKEY	0xFD			//253	375	FD	11111101	ý	&#253;	&yacute;	Latin small letter y with acute
#define MARSHMALLOW_CHATCHAR_TOSEND_YELLOWKEY	0xFE			//254	376	FE	11111110	þ	&#254;	&thorn;	Latin small letter thorn
#define MARSHMALLOW_CHATCHAR_TOSEND_REDKEY	0xFF			//255	377	FF	11111111	ÿ	&#255;	&yuml;	Latin small letter y with diaeresis




// ideas for bot-related macros...
 
// #define Bot_i     Bots[i]
// #define Bot_One   Bots[BOT_1]
// #define Bot_Two   Bots[BOT_2]
// #define Bot_Three Bots[BOT_3]



/*  
// color transforms

// 0 green -1 red, -2 brown, -3 gray, -4 seethru green
#define BOT_GREEN (0)<<MF_TRANSSHIFT
#define BOT_RED (-1)<<MF_TRANSSHIFT
#define BOT_BROWN (-2)<<MF_TRANSSHIFT
#define BOT_GRAY (-3)<<MF_TRANSSHIFT

bot	= P_SpawnMobj (x,y,z, MT_PLAYER);
bot->flags ^= MF_PICKUP;
	if (BotsInGame % 2 == 0)  
	Marshmallow_Bot->flags |= BOT_RED;
else if (BotsInGame % 3 == 0)
	Marshmallow_Bot->flags |= BOT_BROWN;
else if (BotsInGame % 4 == 0)
	Marshmallow_Bot->flags |= BOT_GRAY;
else
	Marshmallow_Bot->flags |= BOT_GREEN;*/
