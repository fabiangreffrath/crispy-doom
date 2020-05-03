#ifndef __MARSHMALLOW__DEFS_H__
#define __MARSHMALLOW__DEFS_H__

// we could probably rename this to network.h

typedef struct {  

	int FriendlyFire;
    int MirrorDamage;
    int SelfDamage;
    int DropGoodies;
    int DropBackpack;
    int CoopItemRespawn;

    int ConservePowerups;
    int KeepWeapons;
    int KeepKeys;
    int SaveItems;

    int GradedWeapons;
    int DeathmatchWeapons;
    int WeaponsStay;

    int AlternateUltraViolence;
    int AlternateNightmare;
    int RespawnInNightmare;

    int TrueInvisibility;

    int PlayerCollision;

    int SandboxMode;

    int Marshmallow_WitholdSSG;
    int Marshmallow_Doom1SSG;
    // ints used to start here
	int SSG_Level;
	int Doom1SSG_Level;

	int physics_mode;

	int upgrade_chance;
	int MonsterHitpointsScale;

	int TreasureMode;     
	
	int dm_timelimit;
	int dm_fraglimit;

	int RandomItems;

	int gibmode;  

	int AlternateLighting;

    int ResizeMonsters;
    int EpicBossDeaths;
    int barrel_fx;
    int DM_AllowExit;
    int DM_KillOnExit;
    int BarrelPushing;
    int DangerousBFG;

	int fliplevels;

} marshmallow_netgame_flags;



#if 0

struct {

	// This struct is just for variables that are either saved or sent somewhere
	// If they aren't in the INI or a netgame option, it doesn't need to be in here 
	// (ex. InfoReadout doesn't need to be in here)
	//
	// and let's skip any IWAD stuff for now

	boolean FriendlyFire;    
	boolean SelfDamage;      
	boolean DropGoodies;    
	boolean DropBackpack;   
	boolean CoopItemRespawn;   
	boolean EndMapNowCheat;  
	boolean GiftDropping; 
	boolean SuicideCheat; 
	boolean KeepKeys; 
	boolean KeepWeapons;  
	boolean DeathmatchWeapons; 
	boolean WeaponsStay;     
	
	boolean DynamicMusic; 
	boolean DJMessages;    
	boolean SongLength;
	boolean FastMusicChanges;

	boolean TrueInvisibility;
	boolean AlternateNightmare;
	boolean RespawnInNightmare;     

	boolean ConservePowerups;     
	boolean SaveItems;     
	boolean GradedWeapons;  

	boolean BoostedChainsaw;
	boolean BoostedFist;
	boolean BoostedBullets; 
	boolean InfinitePistol;
	boolean BalanceShotguns;
	boolean FixChaingunSound;
	boolean PlasmaSlowdown;

	boolean NerfHP_LostSoul;
	boolean NerfHP_Cacodemon;
	boolean NerfHP_Demons;
	boolean NerfPC_Archvile; 
	boolean NerfSpeed_Undead;
	boolean FixChainguySound;
	boolean NerfSkelMissile;
	boolean AltPainDeath;  
	boolean VileZScopeLimit;  
	boolean CacoMeleeSound;   
	
	boolean ExtendedMessages;   
	boolean DeathMessages;     
	boolean DamageMessages;    
	boolean BerserkReminder;    
	boolean ExtraTextLines;    
	boolean PickupMessages;   
	boolean BossAlert;     
	boolean InfightAlert;    
	boolean MissileAlert;     

	boolean ShortRedScreen;      // never used
	boolean BerserkAutoSwitch;     
	
	boolean BFGBlastWave;  
	boolean EngineKillingGibs;  // VERIFY: if we make this "client-side" will we get consistency error?

	int berserk_redscreen_length;   
	int /*Marshmallow_*/ShowTargetHP;   

	int /*Marshmallow_*/TreasureMode;   

} marshmallow_flags;

#endif

#endif