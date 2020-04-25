#ifndef __MARSHMALLOW__   
#define __MARSHMALLOW__

#define MARSHMALLOW_VERSION 076

#include <stdio.h>
#include <stdlib.h>  
#include <math.h>    

#include <SDL.h> 

#include "macros.h"
#include "strings.h"

#include "imports.h"

#include "navigation.h"
#include "bot.h"
#include "dj.h"
#include "hud.h"

#include "profile.h"
//#include "misc.h"

//#include "textscreen.h"

boolean Marshmallow_DynamicMusic; 
boolean Marshmallow_Sandbox;   
boolean Marshmallow_BotEnabled;
boolean Marshmallow_WadStealing;  
boolean Marshmallow_ConsistencyChk;
boolean Marshmallow_PlayingSigil;

boolean Marshmallow_FriendlyFire;   
boolean Marshmallow_MirrorDamage;   
boolean Marshmallow_SelfDamage;     
boolean Marshmallow_DropGoodies;     
boolean Marshmallow_DropBackpack;      
boolean Marshmallow_ShareLoot;  // unused
boolean Marshmallow_CoopItemRespawn;     
boolean Marshmallow_ConservePowerups;    
boolean Marshmallow_KeepWeapons;  
boolean Marshmallow_KeepKeys;    
boolean Marshmallow_SaveItems;     
boolean Marshmallow_GradedWeapons;     
boolean Marshmallow_DeathmatchWeapons;    
boolean Marshmallow_WeaponsStay;     
boolean Marshmallow_DrawWallpaper;     
boolean Marshmallow_AllowSprint;

boolean Marshmallow_TrueInvisibility;
boolean Marshmallow_AlternateNightmare;
boolean Marshmallow_RespawnInNightmare;      
boolean Marshmallow_AlternateUltraViolence;
boolean Marshmallow_BoostedChainsaw;
boolean Marshmallow_BoostedFist;
boolean Marshmallow_BoostedBullets;  // This will affect both pistol and shotgun
boolean Marshmallow_InfinitePistol;
boolean Marshmallow_BalanceShotguns;
//boolean Marshmallow_FixChaingunSound;
boolean Marshmallow_PlasmaSlowdown;
boolean Marshmallow_NerfHP_LostSoul;
boolean Marshmallow_NerfHP_Cacodemon;
boolean Marshmallow_NerfHP_Demons;
boolean Marshmallow_NerfPC_Archvile;
boolean Marshmallow_NerfSpeed_Undead;
boolean Marshmallow_BoostHP_Spiderboss;
//boolean Marshmallow_BoostHP_Chainguy;
boolean Marshmallow_FixChainguySound;
boolean Marshmallow_NerfSkelMissile;
boolean Marshmallow_AltPainDeath;    
boolean Marshmallow_VileZScopeLimit;   
boolean Marshmallow_CacoMeleeSound;    

boolean Marshmallow_InfoReadout;
boolean Marshmallow_ShowBotReadout;    // Only used in botreadout text init 
boolean Marshmallow_ExtendedMessages;    
boolean Marshmallow_DeathMessages;    
boolean Marshmallow_DamageMessages;   
boolean Marshmallow_BerserkReminder;  
boolean Marshmallow_ExtraTextLines;   
boolean Marshmallow_PickupMessages;  
boolean Marshmallow_DJMessages;     
boolean Marshmallow_FastMusicChanges;
boolean Marshmallow_BossAlert;     
boolean Marshmallow_InfightAlert;    
boolean Marshmallow_MissileAlert;     
boolean Marshmallow_LowAmmoWarning; 
boolean Marshmallow_BerserkAutoSwitch; 
boolean Marshmallow_EndMapNowCheat;  
boolean Marshmallow_GiftDropping;
boolean Marshmallow_SuicideCheat;  
boolean Marshmallow_AutoUse;     
boolean Marshmallow_BFGBlastWave; 
boolean Marshmallow_DangerousBFG;

boolean Marshmallow_KeepFreeAmmo;
boolean Marshmallow_PlayerCollision;
boolean Marshmallow_ResizeMonsters;
boolean Marshmallow_PlayerThrust;
boolean Marshmallow_Flashlight;
boolean Marshmallow_TrueRandomDamage;

boolean Marshmallow_BarrelPushing;
boolean Marshmallow_AllowExit;
boolean Marshmallow_KillOnExit;

boolean flashlight_on;
boolean specialnodes;
boolean barrel_fx;

boolean F_Key_BotFollow;

int Marshmallow_RandomItems;  // 0=Off, 1=Just weapons and ammo, 2=Randomize almost everything 

int Marshmallow_GibMode;
boolean Marshmallow_EpicBossDeaths;

int respawn_delay;  // unused

void WADSelectionMenu(void);  // libtextscreen attempt
void RunGUI(void);  // libtextscreen attempt

int SDLWindowTest();  // attempt at SDL window

typedef enum {

	NO_GIBS,
	ROTT_GIBS,
	DUKE_GIBS,
	BRUTAL_GIBS,

} gibmode_t;


typedef enum {

	DEFAULT_THRUST = 1,
	MEDIUM_THRUST_MULTIPLIER = 5,
	HIGH_THRUST_MULTIPLIER = 8,

	NUMPHYSICSMODES = 3
	
} thrust_multipliers_t;
	
int physics_mode;

enum { NORMAL_LIGHTING, CRT_LIGHTING, D3_LIGHTING };
boolean Marshmallow_AlternateLighting;
#define NORMAL_LIGHTLEVEL 1.0
#define DOOM3_LIGHTLEVEL 0.86
#define CRT_LIGHTLEVEL 0.94 
#define NUMLIGHTLEVELS 3
#define MISC_LIGHTLEVEL 0.97

int dm_fraglimit;
int dm_timelimit;
int dm_scores[MAXPLAYERS];

void PKE_Activate();
void PKE_Scanner();

int berserk_redscreen_length;     
int Marshmallow_TreasureMode;     
int Marshmallow_ShowTargetHP;     
int MonsterHitpointsScale;

int newgame_mode;

int marshmallow_tic;

boolean Marshmallow_WitholdSSG; 
boolean Marshmallow_Doom1SSG;   
int SSG_Level;
int Doom1SSG_Level;

boolean we_got_a_cheater;

boolean changing_name;  
char* new_player_name;

skill_t newskill;

#define DOOM2_TITLEPIC 612
#define DOOM1_TITLEPIC 700
#define DOOM2_LOGO 781
#define DOOM1_LOGO 877
//patch_t* titlepic;
//patch_t* logo;
//patch_t* sky1;
//#define DOOM2_SKY 2416
//#define DOOM1_SKY 2000

typedef enum {  

	ICESKULL_REWARD = 10, 
	GOLDSKULL_REWARD = 50, 
	BLOODSKULL_REWARD = 100  

} treasure_score_t;  

// New gibs system: scheduling the gore to happen on the next tic rather than inside PIT_RadiusAttack()
#define MAX_CORPSES 10  // Limit how many corpses we can gib per tick
mobj_t* CorpseList[MAX_CORPSES];
int corpselist_size;

//boolean saved_musicmode;  // for preserving dynamic music preference written to marshmallow.cfg

// network stuff
void LoadMarshmallowSettings(net_gamesettings_t *settings);
void SaveMarshmallowSettings(net_gamesettings_t *settings);
void NET_WriteMarshmallowSettings(net_packet_t *packet, net_gamesettings_t *settings);
boolean NET_ReadMarshmallowSettings(net_packet_t *packet, net_gamesettings_t *settings);
boolean netgamesignal;

void Marshmallow_InitVariables();
void Marshmallow_CheckCommandLineArgs();
void SetCrispyOptions();  // marshmallow
void AddCmdLineBots();    // only in marshmallow.c (ticker)
void InitMusic();   // only in marshmallow.c (ticker)
void HideAllMenus();  // global
void Marshmallow_Controls();  // global
void Marshmallow_Ticker();  // global

// file i/o
#define FILENAME "marshmallow.cfg"  // rename macro
#define PROFILE "profile.cfg"
boolean GetProfile();
void PrepareProfileData(); // profile.c
void CheckForCfgFile();
void CheckForServerCfgFile();
void WritePreferences_OnExit();
//void WriteProfile_OnExit();
void SaveStats();
void DisableStats();
void ResetStats();
void GetSaved_TreasureScore();
void DrawProfile(); 
boolean LaunchProfileScreen();

// profile/stats
int CalculateAccuracy(int shots, int hit);

void Draw_Wallpaper(char* tile);

void Marshmallow_CheckCheats(event_t* ev);  // st_stuff
void Marshmallow_SendMultiplayerEvent(int event);  // hud, lib, marshmallow
boolean Marshmallow_CheckForMultiplayerEvent();  // hu_stuff

boolean	realnetgame;			// used to differentiate between bot games and actual network games
int GetGameType();  // global

void DoTimeouts();  // marshmallow
void DoAllBot_AI();  // marshmallow
void SetWaypointIcon(boolean option_on);  // marshmallow

void HandleRespawnInventory(int player);  // g_game.c
void GiveGradedWeapons(int player);  // g_game.c
void StripWeapons(int player);

void ResetSprint();
void HandleSprint();  // lib
void MightyFistEngaged();  // inter, lib
void PlatformingSprint();  // inter, lib
void BFG_MegaBlast();  // info.c, inter (m)

void LowHealthWarning();   // hud, lib
void UsePortableMedkit();  // hud, lib
void BerserkReminder();  // hud, lib
void LowAmmoWarning();  // hud, inter

boolean pkereadout_on;  // move

// Inventory items
typedef enum {     
	NO_ITEMS,
	ITEM_RADSUIT,
	ITEM_INVUL,
	ITEM_INVIS,
	ITEM_MEDKIT,
	ITEM_VISOR,
	MAX_INV_ITEMS, 

	// We are excluding automap for now
	ITEM_AUTOMAP,

} invitem_t;

void DropInventoryInBackpack(mobj_t* target);
void RecoverInventoryFromBackpack(mobj_t* toucher, mobj_t* special);

void DropWeaponOnPlayerDeath(mobj_t* target); 

typedef enum { RESPAWN_ROCKETS = 2,	RESPAWN_SHELLS = 12, RESPAWN_CELLS = 40, RESPAWN_BULLETS = 50 } respawn_ammo;

// use extern or header:
void PushBarrel();  // p_user, bot_lib
void ResetBarrel();    // marshmallow
void DropBackpackForAFriend(mobj_t* actor); // marshmallow
void PlayerKillsHimself(mobj_t*  actor);  // hud, marshmallow

void AnnounceMostDangerousMonsters(mobj_t* actor); // p_enemy, p_inter, lib
void AnnounceMostDangerousMonstersDeath(mobj_t* actor);  // p_enemy
void Marshmallow_DoMirrorDamage(mobj_t* source, mobj_t* target);  // p_inter
void AnnounceWhoKilledWhat(mobj_t* source, mobj_t* target, player_t* players); // p_inter, lib
boolean Marshmallow_GimmeThatPhatLoot(mobj_t* toucher, mobj_t* special); // p_inter, lib
mobj_t* CreateBackpack(mobj_t* target, boolean is_inventory);  // p_inter
void Marshmallow_NewDropItemsRoutine(mobj_t* target);  // p_inter
boolean CheckPlayerDamage(mobj_t* source, mobj_t* target);  // p_inter

// Auto-use
#define DEFAULT_AUTO_USE_DELAY 32
int AutoUseDelay;
int AutoUseLastX;
int AutoUseLastY;
void AutoUse();

// moved here from macros.h
int		sprint_timeout;  // inter, lib, marshmallow
int		sprint_recharge;  // lib, marshmallow
boolean too_tired_to_sprint; // inter, lib

// function prototypes: dj.c
void HandleMusicOnRespawn();  // p_mobj
void ResetMusicTic();  // s_sound

void DynamicMusic();  // marshmallow
void InitDynamicMusic();  // marshmallow

void ChangeMusicMode();  // hud
void ForcePlaylist(); // hud
char* DisplayPlaylistMode();  // hud
char* ShowSongChanges(); // hud
void ChangeGameMusic();  // hud
char* DisplayPhysicsMode(); // hud
char* DisplayLightingMode(); // hud

boolean DJ_Msg_On();  // dj

void ChangeSongLength();   // defined in dj, called in hud
int GetDangerLevel();  // defined in pkemeter, dj needs it

int PlutoniaSong(int song);  // WIP

// sandbox.c
void InitSandbox(); // p_mobj, hud, lib, marshmallow
void CancelSandbox();
void PlaceMonster(mobj_t* actor);   // marshmallow
void CheckSandboxStatus();  // marshmallow
void UnleashTheHordes();  // marshmallow, lib
int PlaceSandboxItem(int i);  // p_mobj
void ChangeThingType(int player, direction_t direction);  // lib, marshmallow
void RestoreSavedSandbox();
//boolean CheckThingType(mobjtype_t thing);  // static
void ResetSandbox();

// function prototypes: hud.c
void InitHUDMenuText();  // hu_stuff
void DrawHUDMenu();  // hu_stuff
void HUDMenuTicker();  // hu_stuff
void SetMarshmallowColors();  // hu_stuff
void NotifyMissileLock(mobj_t* target);  // p_enemy

// pke widget
boolean hellscape_on; 
hu_stext_t hellscape_text;
#define HELLSCAPE_TEXT_X 280

// pke on datapad
hu_stext_t pkeline1;
hu_stext_t pkeline2;
hu_stext_t pkeline3;
hu_stext_t pkeline4;
hu_stext_t pkeline5;
hu_stext_t pkeline6;
hu_stext_t pkeline7;
hu_stext_t pkeline8;

// missile lock widget
int missilelock_delay;
#define MISSILE_LOCK_TIMEOUT 25
#define MISSILELOCK_X 200
#define MISSILELOCK_Y 15

// public functions for info readouts in hud.c
void PKE_ShowInfo(); 
void PKE_Readout();
void EraseInfoReadout();

// for extra line of text output
hu_stext_t	extra_textline;    // hu_stuff, hud, lib
boolean		extra_textline_on;  // hu_stuff, hud, lib
int			extra_textline_counter;  // hu_stuff

// move these into hud.c:
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

void HUD_InitHelp();       // these have been moved, just need to make them static and re-position in file
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

int skip_to_level;  // map # we're skipping to
void SkipToLevel();
void SkipToRandomLevel();
void ChooseLevel_Next();
void ChooseLevel_Prev();
void SetSkillUpgrades();
void Skill_Next();
void Skill_Prev();
void UpgradeChance_Up();
void UpgradeChance_Down();
void HPScale_Up();
void HPScale_Down();
int RandomMap();

void ToggleSong_Runnin(); 
void ToggleSong_E1M1();  
void ToggleSong_DoomInter();
void ToggleSong_Doom2Inter();

void HUDMenuKeyInput();  // lib
void SetKeyDelay();  // move from lib to hud
void AddToInfoReadout(char* label, int val, int line); // move to hud.c
void AddStringsToInfoReadout(char* label, char* output, int line);  // will be local to hud.c after we move Erase() and DrawPKE() into hud.c

void ShowTargetHP();

void SetOptionsFromCfg();

char* ShowIntAsChar(int val, int color);  
char* ShowFloatAsChar(float val, int color);  

boolean profilescreen_on;  
boolean menus_on;  // g_game, hu_stuff, lib
boolean sandboxhelp_on;  // lib, sandbox
boolean mainmenuhelp_on;
boolean help_on;
boolean mainmenu_on;
boolean weaponmenu_on;
boolean enemymenu_on;
boolean sandboxmenu_on;
boolean skipmenu_on;
boolean musicmenu_on;
boolean vanillamusicmenu_on;
boolean blacklistmenu_on;
boolean invmenu_on;
boolean treasure_on;  // for realnetgame only
boolean skillmenu_on;
boolean optionsmenu_on;
boolean botmenu_on;
boolean msgsmenu_on;
boolean missilelock_on;
boolean botcommandmenu_on;
boolean offer_suicide;
boolean offer_radsuit;
boolean offer_medkit;
int offertimeout_suicide;
int offertimeout_radsuit;
int offertimeout_medkit;
#define DEFAULT_OFFER_TIMEOUT 175   // 5 seconds

boolean CheckKeyDelay();
int menu_key_delay;    // TODO:  rename to menu_scroll_delay
int menu_select_delay;

void DoSharewareBossDeath();

// For plutonia song names fix, though also being used in hud.c
int	current_song;

// math.h
float F_Random();
float GetRandomFloatInRange(float lo, float hi);
int GetRandomIntegerInRange(int lo, int hi);
int GetRandomArrayIndex(int arraySize);
boolean PercentChance(int chance);
void RandomizeIntArray(int *a, int size);
int marshmallow_rndindex;
void Test_Random();
void SeedRandom();  // move to random.c or math.c

// BOT STUFF
int spawntics;  // for delaying use key orders after respawn
int IsBot(player_t* player);  // global
void Bot_PushBarrel(int bot);  // used in p_user.c
void Bot_ExitLevelCleanup(); // g_game.c
void Bot_ResetAll();
void AddBot();  // hud.c and bot
void Bot_RemoveBots(); // hud.c and bot
void SquadRegroup();  // bot_lib.c and lib.c
void Bot_CheckForProblems(int bot);  // bot and bot_lib
void Bot_AssignSingleBotTarget(); // bot_lib and lib
char* ShowBotState(int bot);
void TeleportAnimation(fixed_t x, fixed_t y, fixed_t z);
mobjtype_t waypoint_icon;  
char* botstate[MAXPLAYERS];
#define BOTSTATES_UPDATE_INTERVAL 10

// function prototypes: lib.c

// public:
typedef enum { SHORTBLIP, LONGBLIP } blipsound_t; // move to macros
void PlayBonusSound(blipsound_t length);
boolean CheckVileZScope(mobj_t *actor);
void RestartMap();  // hud, marshmallow
void ToggleFastMonsters();
void CheckDeathmatchStatus();
void CollectTreasure(); // p_inter
void SetPlayerTarget(mobj_t* source, mobj_t* target); // p_inter
void DoWadStealing();  // d_main
int AskForWadStealing();  // d_main
int WadSelection();  // d_main
int CheckForOtherIWAD();
int CheckIfFileExists(const char *filename, const char *mode);
void OfferRadsuit(player_t* player);  // p_spec
mobjtype_t RandomTreasureItem(int i, int probability);  // p_mobj
mobj_t* Marshmallow_InitScaledMonster(mobj_t* monster);  // p_mobj 
boolean IsWeapon(mobj_t* mo);
boolean IsMonster(mobj_t* actor);  // p_enemy, p_inter, p_mobj, bot_lib, inter, pkemeter
boolean IsPlayer(mobj_t* actor);  // p_inter, p_map, inter
boolean IsConsoleplayer(mobj_t* actor);  // p_enemy
boolean PlayerIsDead();  // bot, navigation
boolean Marshmallow_GiveNewInvisPowerup(mobj_t* toucher, mobj_t* special);  // p_inter
void Marshmallow_SetupLevel();  // p_setup
int ColorizeHealth(mobj_t* actor);   // defined in lib, called in lib and bot_lib
int CheckSongBlacklist(int musicnum);  // s_sound
boolean IsBlacklistedSong(int musicnum); 
int GetReplacementSong();
void LaunchHelpWidget();  // m_menu, lib

// consider moving or making public:
void GiveAllItems();  // marshmallow
void SetPunchSound();  // marshmallow
void ToggleFriendlyFire();  // marshmallow, hud
int RandomEpisode(); // hud, marshmallow

int RandomizeItem(int i);

// move:
void LaunchInventoryMenu();  // hud, lib
void ChangeBerserkRedLength();  // move to hud.c
int ShowScore(int player);  // move to hud.c
void ColorizeMapNames();  // move to hud.c
#define SPAWNFARTHEST_DISTANCE MISSILERANGE*0.75

#define NM2_UPGRADECHANCE 90
#define UV2_UPGRADECHANCE 60

//void SendNetgameSignal(int signal);  // VERIFY: shouldn't this be byte type too?   // lib static
byte GetNetgameSignal(int player);   // marshmallow

void DropSlopProp(mobj_t* thing);
boolean IsGibbableThing(mobj_t* thing);
void GibFactory(mobj_t* target, mobj_t* source);
void ParticleFX_EXPBloodSplat(mobj_t *actor);
void ParticleFX_ChainsawBloodSplat(mobj_t *actor);
void ParticleFX_SmokeBlast(mobj_t *actor);
void ParticleFX_SpiderDeath(mobj_t *actor);
void ParticleFX_CyberdemonDeath(mobj_t *actor);
void ParticleFX_BossDeaths(mobj_t* actor);
void ParticleFX_BloodSplat(mobj_t *actor);
void ParticleFX_SmallBloodSplat(mobj_t *actor);
void ParticleFX_Nuclear(mobj_t *actor);
void ParticleFX_HorizontalBlast(mobj_t *actor);
void ParticleFX_SaucerBlast(mobj_t *actor);
void ParticleFX_Test(mobj_t *actor);
void BarrelFX_Test(mobj_t *actor);
void ParticleFX_XDeath(mobj_t *actor);
void BrutalSplat(mobj_t *actor);
void CorpseGib(mobj_t *actor);

int usetimer;
#define DEFAULT_USE_TIMER 20


// UNUSED
void CheckDuplicateLine();  // unused
void Marshmallow_Test_Entry();  // unused
void InitMainMenuText(); // unused
boolean RealNetGame(); // unused
void Marshmallow_SendItemToAllPlayers(mobj_t* special);	// for weapon/key sharing  (unused)
void Marshmallow_GiveNewSoulspherePowerup(mobj_t* toucher, mobj_t* special);  // unused
void CheckForLightTextures();  // WIP experiment...
boolean IsBarrel(mobj_t* actor);  // unused so far
void TreasureRewards();  // unused
void InvertBoolean(boolean option);  // unused
boolean CheckForWeaponsStay(weapontype_t weapon, player_t* player);  // unused
boolean DoDelay(int* delay, int duration);  // unused

// Tweaks
#define DEFAULT_SSG_LEVEL 4
void ToggleGoodiesMode();
void ChangeInvis(boolean option_on); 
boolean Marshmallow_ShadowInvis;

void BoostChainsaw(boolean option_on);
void BoostFist(boolean option_on);
void BoostBullets(boolean option_on);
void InfinitePistol(boolean option_on);
void BalanceShotguns(boolean option_on);
//void FixChaingunSound(boolean option_on);

void NerfHP_LostSoul(boolean option_on);
void NerfHP_Cacodemon(boolean option_on);
void NerfHP_Demons(boolean option_on);
void NerfHP_Archvile(boolean option_on);
void NerfPC_Archvile(boolean option_on);
void NerfSpeed_Undead(boolean option_on);
void NerfHP_BoostHP_Spiderboss(boolean option_on);
void BoostHP_Chainguy(boolean option_on);
void FixChainguySound(boolean option_on);
void ChangePainDeath(boolean option_on);
void NerfSkelMissile(boolean option_on);

int MonsterUpgrade;  // our new difficulty spectrum (from 1-9) for upgrading monsters; the number represents probability of a monster to be upgraded
int UpgradeMonsters(int i);  
int upgrade_chance; 

// weapon balancing
int chainsaw_damage_multiplier;
int fist_damage_multiplier;
int pistol_ammo;  
int shotgun_pellets;
int ssg_range;
int chaingun_sound;
int plasma_rof;
int bfg_ammocost;
int gunshot_damage; 
#define plasma_sputter_threshold 20
#define chaingun_slowdown_threshold 20
#define BOOSTED_CHAINSAW_DAMAGE 2
#define VANILLA_CHAINSAW_DAMAGE 1
//#define FIST_DAMAGE_MULTIPLIER 3 // default is 1
#define EXTRA_PELLETS 9 
#define VANILLA_PELLETS 7
#define SSG_RANGE_NERF 5
int mightyfist_delay;
int punch_sound;
int firingdelay;  // for plasma slowdown
#define VANILLA_FIST_DAMAGE 1
#define BOOSTED_FIST_DAMAGE 2

// tweaks related
void Marshmallow_InitAltNightmare(); // marshmallow, hud
void Marshmallow_InitAltUltraViolence();  // marshmallow
void CheckCurrentSkill(); // global
boolean e3m8_caco_upgrade;  // could be local to tweaks.c

// for keepguns and keepkeys features
boolean Marshmallow_Arsenal[MAXPLAYERS][NUMWEAPONS];  // move these into the player struct  UPDATE: would this get annoying since player is cleared out every time we die?
boolean Marshmallow_KeyRing[MAXPLAYERS][NUMCARDS]; 

void ClearKeyRing(player_t* plyr);  // st_stuff
void SaveArsenal(int player);  // g_game
void SaveKeys(int player);  // g_game
void RestoreArsenal(int player);  // lib static
void RestoreKeyRing(int player);  // lib static

// for graded weapons feature
boolean gaveweapons;  // does not seem to be in use anywhere
boolean organic_levelchange;  // NEW - so we don't get graded weapons when progressing through the levels legitimately

// NOTE: currently hard-coded to 1x which means it's disabled, but there is code I would have to remove if we ditch this
int Marshmallow_LootMultiplier;  

mobjtype_t GetRandomBonusItem(int category); // tweaks
mobjtype_t GenerateRandomBonusItem();
int DistributeDoom2Monsters(int i);  // tweaks
int GetRandomDoom2Monster(int i);
int GetRandomDoom1Monster(int i);


/////////////////////// end tweaks.c

//
// T O D O ...
//
//    see if it's feasible to move into sandbox.h

#define MAX_MONSTERS 1000
#define FIRST_MONSTER 1

typedef struct {

	boolean		design_mode;

	mobj_t*		monster[MAX_MONSTERS];
	mobjtype_t  monster_type[MAX_MONSTERS];
	fixed_t		monster_x[MAX_MONSTERS];
	fixed_t		monster_y[MAX_MONSTERS];
	angle_t		monster_angle[MAX_MONSTERS];
	int			count;
	int			object[MAXPLAYERS];

	int			map;
	int			episode;

	char*		file_name;  
	char*		file_extension;		

} sandbox_t;

sandbox_t sandbox;  // p_enemy, p_inter, p_pspr, p_switch, s_sound, bot, dj, hud, lib, 

sandbox_t saved_sandbox;  // this one might be easier to move


#endif

