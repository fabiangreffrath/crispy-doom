#ifndef __MARSHMALLOW_HUD_
#define __MARSHMALLOW_HUD_

#include "marshmallow.h"

// Coordinates
#define FULLSCREEN_MENU_X_OFFSET 30
#define NEW_X_OFFSET 42  
#define NEW_Y_OFFSET 24
#define INFO_X_OFFSET 150 // TEMPORARY until we separate botreadout and sandboxhelp out of inforeadout 
#define MENU_X 86 - NEW_X_OFFSET       // WAS HU_MSGX + 10 
#define CURSOR_X_MENUS FULLSCREEN_MENU_X_OFFSET+8
#define CURSOR_X_DATAPAD MENU_X-7
#define INV_ICON_X 225
#define INV_ICON_Y 100 -NEW_Y_OFFSET
#define INFO_OUTPUT_X 160
#define INFOREADOUT_Y_OFFSET 100

// Datapad cursor
hu_stext_t menu_cursor;
int cursor_x; // NEW
int cursor_y;
void CalculateCursorPosition();
void StartMenuCursor();

// Bot cursor (activates botreadout widget when facing bot)
int viewingbot;
int lastbot;
int readoutcolor;
boolean botcursor_on;

// Info widget
#define MAX_INFOLINES 8
hu_stext_t miscreadout_output[MAX_INFOLINES];
boolean miscreadout_on;  // move text inits back to hud.c, then this can be local

// TargetHP widget
hu_stext_t targethp_readout;
#define TARGETHP_X 200
#define TARGETHP_Y 30
boolean targethp_on;

// Help widget
#define HELP_DISPLAY_X 10

// Sandbox widget
#define SANDBOX_HELP_THING_X 170
#define SANDBOX_HELP_KEYS_X 200

// Wallpaper/background tile graphic for datapad submenus
#define MENU_WALLPAPER "COMP03_1"

// Functions for hud.c
char* DisplayPhysicsMode();
char* DisplayLightingMode();
char* DisplayMedkitRemaining();
char* DisplayValue(int val);             // these have been moved, just need to make them static and re-position in file
char* DisplayOnOff(boolean option);
char* DisplaySongStatus(boolean option);
char* DisplayMusicMode();
char* DisplayRedScreenMode();
char* DisplayTreasureMode();
char* DisplayGibMode();
char* DisplayCGuySnd(boolean option);
char* DisplayTargetHPSetting(int input);
char* DisplayRevMissileSpeed();
char* DisplayBotSpeed();
char* ShowRandomItemsMode();
char* ShowThingName();
char* ShowMapSelection();
char* ShowMapWeapons();
char* ShowSkillLevel();
char* ShowGameType();
char* ShowSongLength();
char* ShowBotWeapon(int bot);

void HUD_InitHelp();
void HUD_InitEnemyMenu();
void HUD_InitWeaponMenu();
void HUD_InitSandboxMenu();
void HUD_InitSkipLevelMenu();
void HUD_InitMusicMenu();
void HUD_InitMainMenu();
void HUD_InitMessagesMenu();
void HUD_InitSkillMenu();
void HUD_InitInventoryMenu();
void HUD_InitBotMenu();
void HUD_InitGameplayMenu();
void HUD_InitMiscText();
void HUD_InitBlacklistMenu();
void HUD_InitVanillaMusicMenu();
void HUD_InitProfileScreen();

// Cursor selection index for each submenu
int mainmenu_selection; 
int skill_selection;
int options_selection;
int botmenu_selection;
int msgsmenu_selection;
int musicmenu_selection;
int vanillamusicmenu_selection;
int blacklistmenu_selection;
int skipmenu_selection;
int sandboxmenu_selection;
int enemymenu_selection;
int weaponmenu_selection;
int mapweapons_selection;
int treasure_selection;

// Warp weapons options in menu
enum { 
	NO_MAPWEAP_SELECTION,

	GRADED_SELECTED,
	PISTOLSTART_SELECTED,

	MAX_MAPWEAP_OPTIONS,
};

// Weapon tweaks menu
enum {
	NO_WEAPONTWEAK_SELECTION,

	TRUERANDOM_SELECTED,
	BERSAUTO_SELECTED,
	BERSRED_SELECTED,
	CSAWDMG_SELECTED,
	FISTDMG_SELECTED,
	BULDMG_SELECTED,
	INFAMMO_SELECTED,
	SGUNDMG_SELECTED,
	PLSLOW_SELECTED,
	BFGWAVE_SELECTED,

	MAX_WEAPONTWEAKS,
};

// Monster tweaks menu
enum {
	NO_MONSTERTWEAK_SELECTION,

	FASTMONSTERS_SELECTED,
	LOSTSOULHP_SELECTED,
	CACOHP_SELECTED,
	DEMONHP_SELECTED,
	CACOSOUND_SELECTED,
	CGUYSND_SELECTED,
	SKELMISSILE_SELECTED,
	SKELSPEED_SELECTED,
	PAINDEATH_SELECTED,
	VILEPAIN_SELECTED,
	VILEZ_SELECTED,

	MAX_MONSTERTWEAKS,
};

// Sandbox menu
enum {
	NO_SANDBOX_SELECTION,

	WHICHMAP_SELECTED,
	// TODO: LOAD_SELECTED
	// TODO: SAVE_SELECTED
	SANDBOXGO_SELECTED,
	// TODO: add MONSTER HP: 1x, 2x, etc
	RESET_SELECTED,
	//CANCEL_SELECTED,

	MAX_SANDBOX_OPTIONS,
};

// Vanilla music menu
enum {
	NO_VANILLA_SELECTION,
	
	MODE_VANILLA_SELECTED,
	BLACKLIST_VANILLA_SELECTED,

	MAX_VANILLAMUSIC_ITEMS,
};

// Song blacklist menu
enum {
	NO_BLACKLIST_SELECTION,
	
	RUNNIN_SELECTED,
	E1M1_SELECTED,
	D1INTER_SELECTED,
	D2INTER_SELECTED,

	MAX_BLACKLIST_ITEMS,
};

// Dynamic music menu
enum {
	NO_MUSIC_SELECTION,

	MODE_SELECTED,
	SKIP_SELECTED, // now used for song title; TODO: rename
	DJMSGS_SELECTED,
	LENGTH_SELECTED,
	FASTCHANGES_SELECTED,
	PLAYLIST_SELECTED,
	MM_BLACKLIST_SELECTED,
	GAME_SELECTED,

	MAX_MUSICMENU_ITEMS,
};

// Messages menu
enum {
	NO_MSG_SELECTION,

	DANGERINDEX_SELECTED,
	TARGETHP_SELECTED,
	MISSILEALARM_SELECTED,
	PICKUPMSG_SELECTED,
	DEATHMSG_SELECTED,
	BOSSALERTS_SELECTED,
	INFIGHT_SELECTED,
	EXTENDEDMSG_SELECTED,
	EXTRALINE_SELECTED,

	MAX_MSGMENU_ITEMS,
};

// Skill selection in change map menu
enum {

	ITYTD_SELECTED,
	HNTR_SELECTED,
	HMP_SELECTED,
	UV_SELECTED,
	NM_SELECTED,
	UV2_SELECTED,
	NM2_SELECTED,

	MAX_SKILLMENU_ITEMS, 
};

// Gameplay menu
enum {
	NO_OPTION_SELECTION,

	KEEPITEMS_SELECTED,
	CONSERVE_SELECTED,
	GOODIES_SELECTED,
	TREASURE_SELECTED,
	LUDICROUS_SELECTED,
	PHYSICS_SELECTED,
	LIGHTING_SELECTED,
	WEAPONSMENU_SELECTED,
	MONSTERSMENU_SELECTED,
	AUTOUSE_SELECTED,
	SELFDMG_SELECTED,
	KEEPWEAPONS_SELECTED,
	KEEPKEYS_SELECTED,  // show only if (netgame)
	WEAPONSSTAY_SELECTED, // show only if (netgame)

	MAX_GAMEPLAY_OPTIONS,
};

// Bot menu
enum {
	NO_BOT_SELECTION,

	SPAWN_BOT_SELECTED,
	SPAWN_ALL_SELECTED,
	REMOVE_BOTS_SELECTED,
	KILL_BOTS_SELECTED,
	BOT1_WEAPON,
	BOT2_WEAPON,
	BOT3_WEAPON,
	BOTFF_SELECTED,
	//BOTCOLLISION_SELECTED,  // not quite ready for prime time with bots
	BOTSPEED_SELECTED,
	BOTSTATES_SELECTED,

	MAX_BOTMENU_ITEMS,
};

// Change map menu
enum {
	NO_SKIPLEVEL_SELECTION,

	SKIPTONEXT_SELECTED,
	RESTARTMAP_SELECTED,
	RANDOMMAP_SELECTED,
	SKIPTOMAP_SELECTED,
	MAPSKILL_SELECTED,
	MAPSKIPWEAPONS_SELECTED,
	UPGRADE_SELECTED,
	SCALEHP_SELECTED,
	ITEMSPAWNS_SELECTED,
	GOSKIP_SELECTED,

	MAX_SKIPLEVEL_OPTIONS,
};

// Main datapad menu
enum {
    NO_MENU_SELECTION,

    PROFILE_SELECTED,
    OPTIONS_SELECTED,
    BOTMENU_SELECTED,
    MUSIC_SELECTED,
    SKIPMAP_SELECTED,
    SANDBOX_SELECTED,
    MESSAGES_SELECTED,
    MAX_MAINMENU_ITEMS,

    SUICIDE_SELECTED,  // removed from datapad menu
};

// Profile line macros so it's easier to rearrange lines
#define PROFILE_LINE0 profile_favorite_weapon  // removed to save space
#define PROFILE_LINE1 profile_namedisplay
#define PROFILE_LINE2 profile_monsters_killed
#define PROFILE_LINE3 profile_accuracy
#define PROFILE_PERCENTSIGN profile_percent
#define PROFILE_LINE4 profile_treasurescore
#define PROFILE_LINE5 profile_treasureblue
#define PROFILE_LINE6 profile_treasuregold
#define PROFILE_LINE7 profile_treasurered
#define PROFILE_LINE8 profile_levels_completed
#define PROFILE_LINE9 profile_bosses_killed
#define PROFILE_LINE10 profile_secrets_found
#define PROFILE_LINE11 profile_alltreasure
#define PROFILE_LINE12 profile_allkills
#define PROFILE_LINE13 profile_sandbox_kills
#define PROFILE_LINE14 profile_sandbox_games_won

// main menu macros so it's easier to rearrange lines
#define MAINMENU_LINE1	mainmenu_title // unused
#define MAINMENU_LINE2	mainmenu_blank // unused
#define MAINMENU_LINE3	mainmenu_profile
#define MAINMENU_LINE4	mainmenu_options
#define MAINMENU_LINE5  mainmenu_botmenu
#define MAINMENU_LINE6  mainmenu_music
#define MAINMENU_LINE7  mainmenu_nextmap
#define MAINMENU_LINE8	mainmenu_sandbox
#define MAINMENU_LINE9	mainmenu_messages
#define MAINMENU_LINE10 mainmenu_suicide  // removed this line

// Macros for the position of every HUD text line
#define	INV_HU_Y_1	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +18 -NEW_Y_OFFSET))
#define	INV_HU_Y_2	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +26 -NEW_Y_OFFSET))
#define	INV_HU_Y_3	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +34 -NEW_Y_OFFSET))
#define	INV_HU_Y_4	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +42 -NEW_Y_OFFSET))
#define	INV_HU_Y_5	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +50 -NEW_Y_OFFSET))
#define	INV_HU_Y_6	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +58 -NEW_Y_OFFSET))
#define	INV_HU_Y_7	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +66 -NEW_Y_OFFSET))
#define	INV_HU_Y_8	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +74 -NEW_Y_OFFSET))
#define	INV_HU_Y_9	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +82 -NEW_Y_OFFSET))
#define	INV_HU_Y_10	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +90 -NEW_Y_OFFSET))
#define	INV_HU_Y_11	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +98 -NEW_Y_OFFSET))
#define	INV_HU_Y_12	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +106 -NEW_Y_OFFSET))
#define	INV_HU_Y_13	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +114 -NEW_Y_OFFSET))
#define	INV_HU_Y_14	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +122 -NEW_Y_OFFSET))
#define	INV_HU_Y_15	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +130 -NEW_Y_OFFSET))
#define	INV_HU_Y_16	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +138 -NEW_Y_OFFSET))
#define	INV_HU_Y_17	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +146 -NEW_Y_OFFSET))
#define	INV_HU_Y_18	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +154 -NEW_Y_OFFSET))
#define	INV_HU_Y_19	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +162 -NEW_Y_OFFSET))
#define	INV_HU_Y_20	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +170 -NEW_Y_OFFSET))
#define	INV_HU_Y_21	HU_MSGY + (HU_MSGHEIGHT*(SHORT(hu_font[0]->height) +178 -NEW_Y_OFFSET))

// Objects for every line of HUD text
hu_stext_t mainmenu_title;
hu_stext_t mainmenu_blank;
hu_stext_t mainmenu_profile;
hu_stext_t mainmenu_invmenu;
hu_stext_t mainmenu_botmenu;
hu_stext_t mainmenu_suicide;
hu_stext_t mainmenu_nextmap;
hu_stext_t mainmenu_redomap;
hu_stext_t mainmenu_music;
hu_stext_t mainmenu_hellindex;
hu_stext_t mainmenu_messages;
hu_stext_t mainmenu_options;
hu_stext_t mainmenu_sandbox;

hu_stext_t botmenu_title;
hu_stext_t botmenu_blank;
hu_stext_t botmenu_spawn;
hu_stext_t botmenu_spawnall;
hu_stext_t botmenu_states;
hu_stext_t botmenu_remove;
hu_stext_t botmenu_kill;
hu_stext_t botmenu_bot1weapon;
hu_stext_t botmenu_bot2weapon;
hu_stext_t botmenu_bot3weapon;
hu_stext_t botmenu_gamemode;
hu_stext_t botmenu_friendlyfire;
hu_stext_t botmenu_collision;
hu_stext_t botmenu_speed;
hu_stext_t botmenu_statereadout;

hu_stext_t optionsmenu_title;
hu_stext_t optionsmenu_blank;
hu_stext_t optionsmenu_weapons;
hu_stext_t optionsmenu_treasure;
hu_stext_t optionsmenu_monsters;
hu_stext_t optionsmenu_autouse;
hu_stext_t optionsmenu_selfdmg;
hu_stext_t optionsmenu_keepkeys;
hu_stext_t optionsmenu_weaponstay;
hu_stext_t optionsmenu_conserve;
hu_stext_t optionsmenu_keepitems;
hu_stext_t optionsmenu_keepweapons;
hu_stext_t optionsmenu_goodies;
hu_stext_t optionsmenu_ludicrous;
hu_stext_t optionsmenu_lighting;
hu_stext_t optionsmenu_physics;

hu_stext_t skillmenu_title;
hu_stext_t skillmenu_blank;
hu_stext_t skillmenu_baby;
hu_stext_t skillmenu_easy;
hu_stext_t skillmenu_medium;
hu_stext_t skillmenu_hard;
hu_stext_t skillmenu_nightmare;
hu_stext_t skillmenu_nightmare2;
hu_stext_t skillmenu_nmrespawn;

hu_stext_t inv_menu_title;
hu_stext_t inv_menu_empty;
hu_stext_t inv_menu_blank;
hu_stext_t inv_menu_invul;
hu_stext_t inv_menu_invis;
hu_stext_t inv_menu_radsuit;
hu_stext_t inv_menu_medkit;
hu_stext_t inv_menu_visor;
hu_stext_t inv_menu_automap;
hu_stext_t inv_menu_treasure;

hu_stext_t messagesmenu_title;
hu_stext_t messagesmenu_blank;
hu_stext_t messagesmenu_danger;
hu_stext_t messagesmenu_extraline;
hu_stext_t messagesmenu_pickupmsg;
hu_stext_t messagesmenu_deathmsg;
hu_stext_t messagesmenu_extendedmsg;
hu_stext_t messagesmenu_infightmsg;
hu_stext_t messagesmenu_bossmsg;
hu_stext_t messagesmenu_targethp;
hu_stext_t messagesmenu_missile;

hu_stext_t musicmenu_title;
hu_stext_t musicmenu_blank;
hu_stext_t musicmenu_mode;
hu_stext_t musicmenu_skip;
hu_stext_t musicmenu_shuffle;
hu_stext_t musicmenu_msgs;
hu_stext_t musicmenu_length;
hu_stext_t musicmenu_fastchanges;
hu_stext_t musicmenu_playlist;
hu_stext_t musicmenu_game;

hu_stext_t musicmode_vanilla;
hu_stext_t songblacklist_vanilla;

hu_stext_t songblacklist_marshmallow;
hu_stext_t songblacklist_runnin;
hu_stext_t songblacklist_e1m1;
hu_stext_t songblacklist_d1inter;
hu_stext_t songblacklist_d2inter;

hu_stext_t skipmenu_next;
hu_stext_t skipmenu_redo;
hu_stext_t skipmenu_random;
hu_stext_t skipmenu_jump;
hu_stext_t skipmenu_skill;
hu_stext_t skipmenu_weapons;
hu_stext_t skipmenu_upgrade;
hu_stext_t skipmenu_scalehp;
hu_stext_t skipmenu_itemspawns;
hu_stext_t skipmenu_go;

hu_stext_t weaponmenu_bersred;
hu_stext_t weaponmenu_bersauto;
hu_stext_t weaponmenu_csaw;
hu_stext_t weaponmenu_fist;
hu_stext_t weaponmenu_bullet;
hu_stext_t weaponmenu_infammo;
hu_stext_t weaponmenu_shotgun;
hu_stext_t weaponmenu_plasma;
hu_stext_t weaponmenu_bfg;
hu_stext_t weaponmenu_truerandom;

hu_stext_t enemymenu_lostsoul;
hu_stext_t enemymenu_caco;
hu_stext_t enemymenu_demon;
hu_stext_t enemymenu_skel;
hu_stext_t enemymenu_skelmissile;
hu_stext_t enemymenu_paindeath;
hu_stext_t enemymenu_vilepain;
hu_stext_t enemymenu_cguysnd;
hu_stext_t enemymenu_vilez;
hu_stext_t enemymenu_cacosound;
hu_stext_t enemymenu_fast;

hu_stext_t sandboxmenu_map;
hu_stext_t sandboxmenu_go;
hu_stext_t sandboxmenu_reset;
hu_stext_t sandboxmenu_cancel;

hu_stext_t missilelock_text;

hu_stext_t help_titlehelp_line1;
hu_stext_t help_titlehelp_line2;
hu_stext_t help_titlehelp_line3;
hu_stext_t help_title;
hu_stext_t help_mainmenu;
hu_stext_t help_pkemeter;
hu_stext_t help_inventory;
hu_stext_t help_scroll;
hu_stext_t help_select;
hu_stext_t help_menu;
hu_stext_t help_botfollow;
hu_stext_t help_botassign;
hu_stext_t help_botusekey;
hu_stext_t help_botshiftkey;
hu_stext_t help_dropgift;
hu_stext_t help_togglehelp;

hu_stext_t help_sandbox_object;
hu_stext_t help_sandbox_scroll;
hu_stext_t help_sandbox_select;
hu_stext_t help_sandbox_battle;
hu_stext_t help_sandbox_remaining;

hu_stext_t botreadout_state;
hu_stext_t botreadout_chat;
hu_stext_t botreadout_orders;
hu_stext_t botreadout_health;
hu_stext_t botreadout_weapon;

hu_stext_t treasureinfo_line1;
hu_stext_t treasureinfo_line2;

hu_stext_t profile_namedisplay;
hu_stext_t profile_dm_frags;
hu_stext_t profile_accuracy;
hu_stext_t profile_percent;
hu_stext_t profile_kd_ratio;
hu_stext_t profile_shots_fired;
hu_stext_t profile_favorite_weapon;
hu_stext_t profile_monsters_killed;
hu_stext_t profile_players_teamkilled;
hu_stext_t profile_bosses_killed;
hu_stext_t profile_secrets_found;
hu_stext_t profile_sandbox_kills;
hu_stext_t profile_sandbox_games_won;
hu_stext_t profile_sandbox_bosses_killed;
hu_stext_t profile_levels_completed;
hu_stext_t profile_episodes_completed;
hu_stext_t profile_games_completed;
hu_stext_t profile_treasurescore;
hu_stext_t profile_treasureblue;
hu_stext_t profile_treasuregold;
hu_stext_t profile_treasurered;
hu_stext_t profile_alltreasure;
hu_stext_t profile_allkills;


#endif