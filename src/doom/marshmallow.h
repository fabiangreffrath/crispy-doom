#ifndef __MARSHMALLOW__   
#define __MARSHMALLOW__

#include <stdio.h>
#include <stdlib.h>  
#include <math.h>
#include <m_misc.h>

#include <SDL.h> 

#include "macros.h"
#include "strings.h"
#include "defs.h"
#include "imports.h"

#include "navigation.h"
#include "bot.h"
#include "dj.h"
#include "hud.h"
#include "profile.h"

// Global option flags
boolean Marshmallow_DynamicMusic; 
boolean Marshmallow_Sandbox;   
boolean Marshmallow_BotEnabled;
boolean Marshmallow_WadStealing;
boolean Marshmallow_PlayingSigil;
boolean Marshmallow_FriendlyFire;
boolean Marshmallow_MirrorDamage;   
boolean Marshmallow_SelfDamage;     
boolean Marshmallow_DropGoodies;     
boolean Marshmallow_DropBackpack;
boolean Marshmallow_ItemRespawn;
boolean Marshmallow_DeathmatchWeaponsStay;
boolean Marshmallow_ConservePowerups;    
boolean Marshmallow_KeepWeapons;  
boolean Marshmallow_KeepKeys;    
boolean Marshmallow_SaveItems;     
boolean Marshmallow_GradedWeapons;     
boolean Marshmallow_DeathmatchWeapons;    
boolean Marshmallow_WeaponsStay;
boolean Marshmallow_AllowSprint;
boolean Marshmallow_TrueInvisibility;
boolean Marshmallow_AlternateNightmare;
boolean Marshmallow_RespawnInNightmare;      
boolean Marshmallow_AlternateUltraViolence;
boolean Marshmallow_BoostedChainsaw;
boolean Marshmallow_BoostedFist;
boolean Marshmallow_BoostedBullets;
boolean Marshmallow_InfinitePistol;
boolean Marshmallow_BalanceShotguns;
boolean Marshmallow_PlasmaSlowdown;
boolean Marshmallow_NerfHP_LostSoul;
boolean Marshmallow_NerfHP_Cacodemon;
boolean Marshmallow_NerfHP_Demons;
boolean Marshmallow_NerfPC_Archvile;
boolean Marshmallow_NerfSpeed_Undead;
boolean Marshmallow_FixChainguySound;
boolean Marshmallow_NerfSkelMissile;
boolean Marshmallow_AltPainDeath;    
boolean Marshmallow_VileZScopeLimit;   
boolean Marshmallow_CacoMeleeSound;    
boolean Marshmallow_InfoReadout;
boolean Marshmallow_ShowBotReadout;
boolean Marshmallow_ExtendedMessages;    
boolean Marshmallow_DeathMessages;    
boolean Marshmallow_DamageMessages;   
boolean Marshmallow_BerserkReminder;
boolean Marshmallow_DrawWallpaper;
boolean Marshmallow_DatapadGraphics;
boolean Marshmallow_PKEGraphics;
boolean Marshmallow_DatapadSounds;
boolean Marshmallow_PKESounds;
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
boolean Marshmallow_ResizeMonsters;
boolean Marshmallow_Flashlight;
boolean Marshmallow_TrueRandomDamage;
boolean Marshmallow_BarrelPushing;
boolean Marshmallow_ReducedRedscreen;
int Marshmallow_TreasureMode;
int Marshmallow_ShowTargetHP;

// Global functions
void Marshmallow_InitVariables();
void Marshmallow_CheckCommandLineArgs();
void SetCrispyOptions();
void AddCmdLineBots();
void InitMusic();
void HideAllMenus();
void Marshmallow_Controls();
void Marshmallow_Ticker();
void Marshmallow_CheckCheats(event_t* ev);
int GetGameType();
void DoTimeouts();
void DoAllBot_AI();
void SetWaypointIcon(boolean option_on);
void MightyFistEngaged();
void LowHealthWarning();
void UsePortableMedkit();
void BerserkReminder();
void LowAmmoWarning();
void PKE_Activate();
void PKE_Scanner();
void PushBarrel();
void ResetBarrel();
void PlayerKillsHimself(mobj_t*  actor);
void AnnounceMostDangerousMonsters(mobj_t* actor);
void AnnounceMostDangerousMonstersDeath(mobj_t* actor);
void AnnounceWhoKilledWhat(mobj_t* source, mobj_t* target, player_t* players);
void Marshmallow_NewDropItemsRoutine(mobj_t* target);
void Marshmallow_DoMirrorDamage(mobj_t* source, mobj_t* target);
boolean CheckPlayerDamage(mobj_t* source, mobj_t* target);
void LaunchInventoryMenu();
void ChangeBerserkRedLength();
void PlayBonusSound(blipsound_t length);
boolean CheckVileZScope(mobj_t *actor);
void RestartMap();
void ToggleFastMonsters();
void CheckDeathmatchStatus();
void CollectTreasure(treasure_t color, mobj_t* toucher);
void ResetTreasure();
void SetPlayerTarget(mobj_t* source, mobj_t* target);
void DoWadStealing();
int AskForWadStealing();
int WadSelection();
int CheckForOtherIWAD();
int CheckIfFileExists(const char *filename, const char *mode);
void OfferRadsuit(player_t* player);
mobjtype_t RandomTreasureItem(int i, int probability);
void Marshmallow_InitScaledMonster(mobj_t* monster);
boolean IsWeapon(mobj_t* mo);
boolean IsMonster(mobj_t* actor);
boolean IsPlayer(mobj_t* actor);
boolean IsBarrel(mobj_t* actor);
boolean IsConsoleplayer(mobj_t* actor);
boolean PlayerIsDead();
boolean Marshmallow_GiveNewInvisPowerup(mobj_t* toucher, mobj_t* special);
void Marshmallow_SetupLevel();
int ColorizeHealth(mobj_t* actor);
int CheckSongBlacklist(int musicnum);
boolean IsBlacklistedSong(int musicnum);
int GetReplacementSong();
void LaunchHelpWidget();
void IncrementShotsFired(player_t* player);
void RegisterShotsHit(mobj_t* inflictor, mobj_t* target);
void InfightAlert(mobj_t* actor);
void GiveAllItems();
void SetPunchSound();
void ToggleFriendlyFire();
int RandomEpisode();
void SetSkills();
void DisplayMedkitRemaining();
void SetDMFlags();
void KilledByPlayer(mobj_t* source, mobj_t* target);
void KilledPlayer(mobj_t* source, mobj_t* target);
void WriteToSecondConsoleLine(char* string, int timeout);
void WriteToThirdConsoleLine(char* string, int timeout);
void WriteToFourthConsoleLine(char* string, int timeout);
void SetOptionsFromCfg();
int skip_to_level;  // Map # we're skipping to
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
void AddIntegerToInfoReadout(char* label, int val, int line);
void AddStringsToInfoReadout(char* label, char* output, int line);
void ShowTargetHP();
void PKE_Readout();
void HUDMenuKeyInput();
void Advance_M_Tics();
void HandleNetgameEvents();
void Player_Actions();
void Game_Actions();
void Game_Init();
boolean PhysicsExempt(mobj_t* thing);
boolean EnemyInRange(mobj_t* enemy);
void PlayMenuSound(int sound);

// Network stuff
char* marshmallow_player_names[4];
boolean netgamesignal;
boolean Marshmallow_CheckForMultiplayerEvent();
boolean	realnetgame;	// Used to differentiate between bot games and actual network games
void LoadMarshmallowSettings(net_gamesettings_t *settings);
void SaveMarshmallowSettings(net_gamesettings_t *settings);
void NET_WriteMarshmallowSettings(net_packet_t *packet, net_gamesettings_t *settings);
boolean NET_ReadMarshmallowSettings(net_packet_t *packet, net_gamesettings_t *settings);
byte GetNetgameSignal(int player);
void Marshmallow_SendMultiplayerEvent(int event);

// Saving and loading to config files
#define FILENAME "marshmallow.cfg"  // TODO: rename macro to CFG_FILE
#define PROFILE "profile.cfg"
boolean GetProfile();
void PrepareProfileData();
void CheckForCfgFile();
void CheckForServerCfgFile();
void WritePreferences_OnExit();
void SaveStats();
void DisableStats();
void ResetStats();
void DrawProfile(); 
boolean LaunchProfileScreen();
int PKE_SavedSearchRadius;

// Profile and stats
int CalculateAccuracy(int shots, int hit);
void Draw_Wallpaper(char* tile);
boolean we_got_a_cheater;

// Alternate physics modes
int physics_mode;

typedef enum {

    DEFAULT_THRUST = 1,
    MEDIUM_THRUST_MULTIPLIER = 5,
    HIGH_THRUST_MULTIPLIER = 8,

    NUMPHYSICSMODES = 3

} thrust_multipliers_t;

// Alternate lighting modes
enum { NORMAL_LIGHTING, CRT_LIGHTING, D3_LIGHTING };
int Marshmallow_AlternateLighting;
#define NORMAL_LIGHTLEVEL 1.0
#define DOOM3_LIGHTLEVEL 0.86
#define CRT_LIGHTLEVEL 0.94
#define NUMLIGHTLEVELS 3
#define MISC_LIGHTLEVEL 0.97

// DM options
int dm_fraglimit;
int dm_timelimit;
int dm_scoreboard;
int dm_scores[MAXPLAYERS];
int Preferred_DM_Mode;
boolean Marshmallow_AllowExit;
boolean Marshmallow_KillOnExit;

// Backpack and weapon drops
void DropInventoryInBackpack(mobj_t* target);
void RecoverInventoryFromBackpack(mobj_t* toucher, mobj_t* special);
void DropWeaponOnPlayerDeath(mobj_t* target);
void DropBackpackForAFriend(mobj_t* actor);
boolean Marshmallow_GimmeThatPhatLoot(mobj_t* toucher, mobj_t* special);
mobj_t* CreateBackpack(mobj_t* target, boolean is_inventory);

// Globals for Doom DJ
void HandleMusicOnRespawn();
void ResetMusicTic();
void DynamicMusic();
void InitDynamicMusic();
void ChangeMusicMode();
void ForcePlaylist();
char* DisplayPlaylistMode();
char* ShowSongChanges();
void ChangeGameMusic();
void ChangeSongLength();
boolean DJ_Msg_On();

// Global sandbox functions
void InitSandbox();
void CancelSandbox();
void PlaceMonster(mobj_t* actor);
void CheckSandboxStatus();
void UnleashTheHordes();
int PlaceSandboxItem(int i);
void ChangeThingType(int player, direction_t direction);
void RestoreSavedSandbox();
void ResetSandbox();

// Global HUD functions
void InitHUDMenuText();
void DrawHUDMenu();
void HUDMenuTicker();
void SetMarshmallowColors();
void NotifyMissileLock(mobj_t* target);
int ShowScore(int player);
void ColorizeMapNames();

// PKE widget
boolean hellscape_on; 
hu_stext_t hellscape_text;
#define HELLSCAPE_TEXT_X 280

// PKE meter on datapad
hu_stext_t pkeline1;
hu_stext_t pkeline2;
hu_stext_t pkeline3;
hu_stext_t pkeline4;
hu_stext_t pkeline8;

// On/off flags for every submenu
boolean shortcutmenu_on;
boolean pkereadout_on;
boolean profilescreen_on;  
boolean menus_on;
boolean sandboxhelp_on;
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
boolean treasure_on;  // Used for realnetgame only
boolean skillmenu_on;
boolean optionsmenu_on;
boolean deathmatchmenu_on;
boolean botmenu_on;
boolean msgsmenu_on;
boolean missilelock_on;
boolean botcommandmenu_on;
boolean offer_suicide;
boolean offer_radsuit;
boolean offer_medkit;
boolean second_consoleline_on;
boolean third_consoleline_on;
boolean fourth_consoleline_on;
int second_consoleline_timeout;
int third_consoleline_timeout;
int fourth_consoleline_timeout;
#define DEFAULT_EXTRALINE_TIMEOUT 250
#define SHORT_EXTRALINE_TIMEOUT 150

// Inventory menu
boolean WeHaveItem(invitem_t item);
int invmenu_selection;

enum {
    NO_SELECTION,

    RADSUIT_SELECTED,
    INVUL_SELECTED,
    INVIS_SELECTED,
    MEDKIT_SELECTED,
    VISOR_SELECTED,
    //AUTOMAP_SELECTED,

    MAX_INV_OPTIONS,
} ;

// New math/randomization functions
float F_Random();
float GetRandomFloatInRange(float lo, float hi);
int GetRandomIntegerInRange(int lo, int hi);
int GetRandomArrayIndex(int arraySize);
boolean PercentChance(int chance);
void RandomizeIntArray(int *a, int size);
int marshmallow_rndindex;
void Test_Random();
void SeedRandom();

// Globals for bots
int usetimer;
#define DEFAULT_USE_TIMER 20
int spawntics;  // for delaying use key orders after respawn
int IsBot(player_t* player);
void Bot_PushBarrel(int bot);
void Bot_ExitLevelCleanup();
void Bot_ResetAll();
void AddBot();
void Bot_RemoveBots();
void SquadRegroup();
void Bot_CheckForProblems(int bot);
void Bot_AssignSingleBotTarget();
char* ShowBotState(int bot);
void TeleportAnimation(fixed_t x, fixed_t y, fixed_t z);
mobjtype_t waypoint_icon;  
char* botstate[MAXPLAYERS];
#define BOTSTATES_UPDATE_INTERVAL 10
boolean F_Key_BotFollow;

// Gore/particles
int Marshmallow_GibMode;
boolean Marshmallow_EpicBossDeaths;
boolean barrel_fx;  // TODO: rename
void PlaySlopSound(mobj_t* target);
boolean IsGibbableThing(mobj_t* thing);
void GibActor(mobj_t* target, mobj_t* source);
void CorpseGib(mobj_t *actor);
void BrutalSplat(mobj_t *actor);
void ParticleFX_EXPBloodSplat(mobj_t *actor);
void ParticleFX_ChainsawBloodSplat(mobj_t *actor);
void ParticleFX_SmokeBlast(mobj_t *actor);
void ParticleFX_SpiderDeath(mobj_t *actor);
void ParticleFX_CyberdemonDeath(mobj_t *actor);
void DoBossExplosions(mobj_t* actor);
void ParticleFX_BloodSplat(mobj_t *actor);
void ParticleFX_SmallBloodSplat(mobj_t *actor);
void ParticleFX_TinyBloodSplat(mobj_t *actor);
void ParticleFX_Nuclear(mobj_t *actor);
void ParticleFX_HorizontalBlast(mobj_t *actor);
void ParticleFX_SaucerBlast(mobj_t *actor);
void ParticleFX_BabyDeath(mobj_t *actor);
void ParticleFX_Test(mobj_t *actor);
void BarrelFX_Test(mobj_t *actor);
void ParticleFX_XDeath(mobj_t *actor);
void BFG_MegaBlast(mobj_t *actor);
void DoChainsawBlood(mobj_t* target, mobj_t* inflictor);
void DoCriticalHitBlood(mobj_t* target);
boolean DoSpecialDeaths(mobj_t* target, mobj_t* source);
#define GIB_CHANCE 60
#define CRITICALHIT_THRESHOLD (target->info->spawnhealth/4)

// Monster tweaks
int chaingunguy_attack_sound;
int vile_damage;
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

// Weapon tweaks
void BoostChainsaw(boolean option_on);
void BoostFist(boolean option_on);
void BoostBullets(boolean option_on);
void InfinitePistol(boolean option_on);
void BalanceShotguns(boolean option_on);
int berserk_redscreen_length;
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

// Delay SSG feature
boolean Marshmallow_WitholdSSG;
boolean Marshmallow_Doom1SSG;
#define DEFAULT_SSG_LEVEL 4
int SSG_Level;
int Doom1SSG_Level;

// New skill levels
int upgrade_chance;  // Our new difficulty spectrum (from 1-9) for upgrading monsters; the number represents probability of a monster to be upgraded
int UpgradeMonsters(int i);
void Marshmallow_InitAltNightmare();
void Marshmallow_InitAltUltraViolence();
boolean e3m8_caco_upgrade;
#define NM2_UPGRADECHANCE 90
#define UV2_UPGRADECHANCE 60
int MonsterHitpointsScale;

// Goodies mode
void ToggleGoodiesMode();
void ChangeInvis(boolean option_on);
boolean Marshmallow_ShadowInvis;

// KeepWeapons and KeepKeys data
typedef enum { RESPAWN_ROCKETS = 2,	RESPAWN_SHELLS = 12, RESPAWN_CELLS = 40, RESPAWN_BULLETS = 50 } respawn_ammo;
boolean Marshmallow_Arsenal[MAXPLAYERS][NUMWEAPONS];
boolean Marshmallow_KeyRing[MAXPLAYERS][NUMCARDS];
void ClearKeyRing(player_t* plyr);
void SaveArsenal(int player);
void SaveKeys(int player);
void RestoreArsenal(int player);
void RestoreKeyRing(int player);
void HandleRespawnInventory(int player);

// Warp Weapons feature
boolean gaveweapons;  // does not seem to be in use anywhere
boolean organic_levelchange;  // NEW - so we don't get graded weapons when progressing through the levels legitimately
void GiveGradedWeapons(int player);
void StripWeapons(int player);

// Item and monster randomization
int Marshmallow_RandomItems;  // 0=Off, 1=Just weapons and ammo, 2=Randomize almost everything
int RandomizeItem(int i);
mobjtype_t GetRandomBonusItem(int category);
mobjtype_t GenerateRandomBonusItem();
int GetRandomDoom2Monster(int i);
int GetRandomDoom1Monster(int i);
int itemrespawn_delay;
#define DEFAULT_ITEMRESPAWN_DELAY 30

// Auto-use
#define DEFAULT_AUTO_USE_DELAY 32
int AutoUseDelay;
int AutoUseLastX;
int AutoUseLastY;
void AutoUse();

// Misc
int offertimeout_suicide;
int offertimeout_radsuit;
int offertimeout_medkit;
#define DEFAULT_OFFER_TIMEOUT 175   // 5 seconds
int marshmallow_tic;  // Our own ticker that resets on map changes
typedef int playerindex_t;  // Somewhat pointless
int newgame_mode;
skill_t newskill;
void SetKeyDelay();
boolean CheckKeyDelay();
int menu_key_delay;    // TODO:  rename to menu_scroll_delay

// Global sandbox data
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

sandbox_t sandbox;
sandbox_t saved_sandbox;

//
// Un-implemented features below
//

// Misc
int Marshmallow_LootMultiplier;  // Remains hard-coded to 1
boolean flashlight_on;  // skipped feature
boolean specialnodes;  // probably not in use

// Name change in-game
boolean changing_name;  // unused/removed
//char* new_player_name;

// Sprint feature
int		sprint_timeout;  // inter, lib, marshmallow
int		sprint_recharge;  // lib, marshmallow
boolean too_tired_to_sprint; // inter, lib
void ResetSprint();
void HandleSprint();
void PlatformingSprint();

// Extra line of text output
hu_stext_t	extra_textline;    // hu_stuff, hud, lib
boolean		extra_textline_on;  // hu_stuff, hud, lib
int			extra_textline_counter;  // hu_stuff

#endif

