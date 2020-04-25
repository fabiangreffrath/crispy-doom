////////////////////////////////////////////////
//
//  FILEIO.C:  Read/write settings to .cfg file
//
////////////////////////////////////////////////

#include "marshmallow.h"


static void ReadServerSettings(FILE *f)
{
	fscanf(f, "FriendlyFire %d \n", &Marshmallow_FriendlyFire); 
	fscanf(f, "SelfDamage %d \n", &Marshmallow_SelfDamage);
	fscanf(f, "DropGoodies %d \n", &Marshmallow_DropGoodies);
	fscanf(f, "DropBackpack %d \n", &Marshmallow_DropBackpack);
	fscanf(f, "EndMapNowCheat %d \n", &Marshmallow_EndMapNowCheat);
	fscanf(f, "SuicideCheat %d \n", &Marshmallow_SuicideCheat);
	fscanf(f, "KeepKeys %d \n", &Marshmallow_KeepKeys);
	fscanf(f, "KeepWeapons %d \n", &Marshmallow_KeepWeapons);
	fscanf(f, "WeaponsStay %d \n", &Marshmallow_WeaponsStay);
	
	fscanf(f, "TrueInvisibility %d \n", &Marshmallow_TrueInvisibility);
	fscanf(f, "ConservePowerups %d \n", &Marshmallow_ConservePowerups);

	fscanf(f, "GradedWeapons %d \n", &Marshmallow_GradedWeapons);
	fscanf(f, "BoostedChainsaw %d \n", &Marshmallow_BoostedChainsaw);
	fscanf(f, "BoostedFist %d \n", &Marshmallow_BoostedFist);
	fscanf(f, "BoostedBullets %d \n", &Marshmallow_BoostedBullets);
	fscanf(f, "InfinitePistol %d \n", &Marshmallow_InfinitePistol);
	fscanf(f, "BalanceShotguns %d \n", &Marshmallow_BalanceShotguns);

	fscanf(f, "NerfHP_LostSoul %d \n", &Marshmallow_NerfHP_LostSoul);
	fscanf(f, "NerfHP_Cacodemon %d \n", &Marshmallow_NerfHP_Cacodemon);
	fscanf(f, "NerfHP_Demons %d \n", &Marshmallow_NerfHP_Demons);
	fscanf(f, "NerfPC_Archvile %d \n", &Marshmallow_NerfPC_Archvile);  
	fscanf(f, "NerfSpeed_Undead %d \n", &Marshmallow_NerfSpeed_Undead);
	fscanf(f, "FixChainguySound %d \n", &Marshmallow_FixChainguySound);
	fscanf(f, "NerfSkelMissile %d \n", &Marshmallow_NerfSkelMissile);
	fscanf(f, "AltPainDeath %d \n", &Marshmallow_AltPainDeath);
	fscanf(f, "VileZScopeLimit %d \n", &Marshmallow_VileZScopeLimit);
	fscanf(f, "CacoMeleeSound %d \n", &Marshmallow_CacoMeleeSound);

	fscanf(f, "ExtendedMessages %d \n", &Marshmallow_ExtendedMessages);
	fscanf(f, "DeathMessages %d \n", &Marshmallow_DeathMessages);
	fscanf(f, "DamageMessages %d \n", &Marshmallow_DamageMessages);
	fscanf(f, "BerserkReminder %d \n", &Marshmallow_BerserkReminder);  
	fscanf(f, "ExtraTextLines %d \n", &Marshmallow_ExtraTextLines);
	fscanf(f, "PickupMessages %d \n", &Marshmallow_PickupMessages);
	fscanf(f, "BossAlert %d \n", &Marshmallow_BossAlert);
	fscanf(f, "InfightAlert %d \n", &Marshmallow_InfightAlert);
	fscanf(f, "MissileAlert %d \n", &Marshmallow_MissileAlert);
	fscanf(f, "ShowPKEWidget %d \n", &hellscape_on);  
	
	fscanf(f, "BerserkAutoSwitch %d \n", &Marshmallow_BerserkAutoSwitch);
	fscanf(f, "BFGBlastWave %d \n", &Marshmallow_BFGBlastWave);

	fscanf(f, "GibMode %d \n", &Marshmallow_GibMode);  

	fscanf(f, "berserk_redscreen_length %d \n", &berserk_redscreen_length);
	fscanf(f, "ShowTargetHP %d \n", &Marshmallow_ShowTargetHP);
	fscanf(f, "TreasureMode %d \n", &Marshmallow_TreasureMode);

	fscanf(f, "DrawWallpaper %d \n", &Marshmallow_DrawWallpaper);
	fscanf(f, "dm_fraglimit %d \n", &dm_fraglimit);
	fscanf(f, "dm_timelimit %d \n", &dm_timelimit);
	fscanf(f, "physics_mode %d \n", &physics_mode);
	fscanf(f, "KeepFreeAmmo %d \n", &Marshmallow_KeepFreeAmmo);
	fscanf(f, "SSG_Level %d \n", &SSG_Level);
	fscanf(f, "AlternateLighting %d \n", &Marshmallow_AlternateLighting);
	fscanf(f, "ResizeMonsters %d \n", &Marshmallow_ResizeMonsters);

	fscanf(f, "EpicBossDeaths %d \n", &Marshmallow_EpicBossDeaths);
	fscanf(f, "barrel_fx %d \n", &barrel_fx);

	fscanf(f, "DM_AllowExit %d \n", &Marshmallow_AllowExit);
	fscanf(f, "DM_KillOnExit %d \n", &Marshmallow_KillOnExit);
	fscanf(f, "DeathmatchWeapons %d \n", &Marshmallow_DeathmatchWeapons);

	fscanf(f, "BarrelPushing %d \n", &Marshmallow_BarrelPushing);
	fscanf(f, "DangerousBFG %d \n", &Marshmallow_DangerousBFG);
}


void CheckForServerCfgFile()
{
	FILE *cfg_file;
	//float cfg_version;

	cfg_file = fopen("server.cfg", "r");

	if (!cfg_file)
	{
		//WritePreferences();  // write default settings here if that's what we want to do
		return;
	}

	// Load saved preferences from marshmallow.cfg
	ReadServerSettings(cfg_file);
}


static void WritePreferences()    
{
	FILE *f;
	f = fopen(FILENAME, "w");

	if (!f)
		return;

	fprintf(f, "FriendlyFire %d \n", Marshmallow_FriendlyFire);
	fprintf(f, "SelfDamage %d \n", Marshmallow_SelfDamage);
	fprintf(f, "DropGoodies %d \n", Marshmallow_DropGoodies);
	fprintf(f, "DropBackpack %d \n", Marshmallow_DropBackpack);
	fprintf(f, "EndMapNowCheat %d \n", Marshmallow_EndMapNowCheat);
	fprintf(f, "SuicideCheat %d \n", Marshmallow_SuicideCheat);
	fprintf(f, "KeepKeys %d \n", Marshmallow_KeepKeys);
	fprintf(f, "KeepWeapons %d \n", Marshmallow_KeepWeapons);
	fprintf(f, "WeaponsStay %d \n", Marshmallow_WeaponsStay);

	fprintf(f, "DynamicMusic %d \n", Marshmallow_DynamicMusic);
	fprintf(f, "DJMessages %d \n", Marshmallow_DJMessages);
	fprintf(f, "SongLength %d \n", Doom_DJ.song_length);
	fprintf(f, "SkipSong_E1M1 %d \n", song_blacklist[BLACKLIST_E1M1]);
	fprintf(f, "SkipSong_MAP01 %d \n", song_blacklist[BLACKLIST_RUNNIN]);
	fprintf(f, "SkipSong_D1INT %d \n", song_blacklist[BLACKLIST_D1INTER]);
	fprintf(f, "SkipSong_D2INT %d \n", song_blacklist[BLACKLIST_D2INTER]);

	fprintf(f, "TrueInvisibility %d \n", Marshmallow_TrueInvisibility);

	fprintf(f, "ConservePowerups %d \n", Marshmallow_ConservePowerups);
	fprintf(f, "SaveItems %d \n", Marshmallow_SaveItems);
	fprintf(f, "GradedWeapons %d \n", Marshmallow_GradedWeapons);

	fprintf(f, "BoostedChainsaw %d \n", Marshmallow_BoostedChainsaw);
	fprintf(f, "BoostedFist %d \n", Marshmallow_BoostedFist);
	fprintf(f, "BoostedBullets %d \n", Marshmallow_BoostedBullets);
	fprintf(f, "InfinitePistol %d \n", Marshmallow_InfinitePistol);
	fprintf(f, "BalanceShotguns %d \n", Marshmallow_BalanceShotguns);
	fprintf(f, "PlasmaSlowdown %d \n", Marshmallow_PlasmaSlowdown);

	fprintf(f, "NerfHP_LostSoul %d \n", Marshmallow_NerfHP_LostSoul);
	fprintf(f, "NerfHP_Cacodemon %d \n", Marshmallow_NerfHP_Cacodemon);
	fprintf(f, "NerfHP_Demons %d \n", Marshmallow_NerfHP_Demons);
	fprintf(f, "NerfPC_Archvile %d \n", Marshmallow_NerfPC_Archvile);  
	fprintf(f, "NerfSpeed_Undead %d \n", Marshmallow_NerfSpeed_Undead);
	fprintf(f, "FixChainguySound %d \n", Marshmallow_FixChainguySound);
	fprintf(f, "NerfSkelMissile %d \n", Marshmallow_NerfSkelMissile);
	fprintf(f, "AltPainDeath %d \n", Marshmallow_AltPainDeath);
	fprintf(f, "VileZScopeLimit %d \n", Marshmallow_VileZScopeLimit);
	fprintf(f, "CacoMeleeSound %d \n", Marshmallow_CacoMeleeSound);

	fprintf(f, "ExtendedMessages %d \n", Marshmallow_ExtendedMessages);
	fprintf(f, "DeathMessages %d \n", Marshmallow_DeathMessages);
	fprintf(f, "DamageMessages %d \n", Marshmallow_DamageMessages);
	fprintf(f, "BerserkReminder %d \n", Marshmallow_BerserkReminder);  
	fprintf(f, "ExtraTextLines %d \n", Marshmallow_ExtraTextLines);
	fprintf(f, "PickupMessages %d \n", Marshmallow_PickupMessages);
	fprintf(f, "BossAlert %d \n", Marshmallow_BossAlert);
	fprintf(f, "InfightAlert %d \n", Marshmallow_InfightAlert);
	fprintf(f, "MissileAlert %d \n", Marshmallow_MissileAlert);
	fprintf(f, "ShowPKEWidget %d \n", hellscape_on);   
	
	fprintf(f, "BerserkAutoSwitch %d \n", Marshmallow_BerserkAutoSwitch);
	fprintf(f, "BFGBlastWave %d \n", Marshmallow_BFGBlastWave);
	fprintf(f, "GibMode %d \n", Marshmallow_GibMode);
	fprintf(f, "berserk_redscreen_length %d \n", berserk_redscreen_length);
	fprintf(f, "ShowTargetHP %d \n", Marshmallow_ShowTargetHP);
	fprintf(f, "TreasureMode %d \n", Marshmallow_TreasureMode);
	fprintf(f, "FastMusicChanges %d \n", Marshmallow_FastMusicChanges);

	fprintf(f, "DrawWallpaper %d \n", Marshmallow_DrawWallpaper);
	fprintf(f, "dm_fraglimit %d \n", dm_fraglimit);
	fprintf(f, "dm_timelimit %d \n", dm_timelimit);
	fprintf(f, "physics_mode %d \n", physics_mode);
	fprintf(f, "KeepFreeAmmo %d \n", Marshmallow_KeepFreeAmmo);
	fprintf(f, "SSG_Level %d \n", SSG_Level);
	fprintf(f, "AlternateLighting %d \n", Marshmallow_AlternateLighting);
	fprintf(f, "ResizeMonsters %d \n", Marshmallow_ResizeMonsters);
	fprintf(f, "Flashlight %d \n", Marshmallow_Flashlight);
	fprintf(f, "ShowBotStates %d \n", ShowBotStates);
	//fprintf(f, "default_bot_speed %d \n", default_bot_speed);
	fprintf(f, "TrueRandomDamage %d \n", Marshmallow_TrueRandomDamage);
	fprintf(f, "EpicBossDeaths %d \n", Marshmallow_EpicBossDeaths);
	fprintf(f, "barrel_fx %d \n", barrel_fx);

	fprintf(f, "F_Key_BotFollow %d \n", F_Key_BotFollow);
	fprintf(f, "AllowSprint %d \n", Marshmallow_AllowSprint);

	fprintf(f, "BarrelPushing %d \n", Marshmallow_BarrelPushing);
	fprintf(f, "DangerousBFG %d \n", Marshmallow_DangerousBFG);
	fprintf(f, "DM_AllowExit %d \n", Marshmallow_AllowExit);
	fprintf(f, "DM_KillOnExit %d \n", Marshmallow_KillOnExit);

	fclose(f);
}


static void LoadPreferences(FILE *f)  
{
	fscanf(f, "FriendlyFire %d \n", &Marshmallow_FriendlyFire); 
	fscanf(f, "SelfDamage %d \n", &Marshmallow_SelfDamage);
	fscanf(f, "DropGoodies %d \n", &Marshmallow_DropGoodies);
	fscanf(f, "DropBackpack %d \n", &Marshmallow_DropBackpack);
	fscanf(f, "EndMapNowCheat %d \n", &Marshmallow_EndMapNowCheat);
	fscanf(f, "SuicideCheat %d \n", &Marshmallow_SuicideCheat);
	fscanf(f, "KeepKeys %d \n", &Marshmallow_KeepKeys);
	fscanf(f, "KeepWeapons %d \n", &Marshmallow_KeepWeapons);
	fscanf(f, "WeaponsStay %d \n", &Marshmallow_WeaponsStay);


	fscanf(f, "DynamicMusic %d \n", &Marshmallow_DynamicMusic);	
	fscanf(f, "DJMessages %d \n", &Marshmallow_DJMessages);
	fscanf(f, "SongLength %d \n", &Doom_DJ.song_length);
	fscanf(f, "SkipSong_E1M1 %d \n", &song_blacklist[BLACKLIST_E1M1]);
	fscanf(f, "SkipSong_MAP01 %d \n", &song_blacklist[BLACKLIST_RUNNIN]);
	fscanf(f, "SkipSong_D1INT %d \n", &song_blacklist[BLACKLIST_D1INTER]);
	fscanf(f, "SkipSong_D2INT %d \n", &song_blacklist[BLACKLIST_D2INTER]);

	fscanf(f, "TrueInvisibility %d \n", &Marshmallow_TrueInvisibility);

	fscanf(f, "ConservePowerups %d \n", &Marshmallow_ConservePowerups);
	fscanf(f, "SaveItems %d \n", &Marshmallow_SaveItems);
	fscanf(f, "GradedWeapons %d \n", &Marshmallow_GradedWeapons);

	fscanf(f, "BoostedChainsaw %d \n", &Marshmallow_BoostedChainsaw);
	fscanf(f, "BoostedFist %d \n", &Marshmallow_BoostedFist);
	fscanf(f, "BoostedBullets %d \n", &Marshmallow_BoostedBullets);
	fscanf(f, "InfinitePistol %d \n", &Marshmallow_InfinitePistol);
	fscanf(f, "BalanceShotguns %d \n", &Marshmallow_BalanceShotguns);
	fscanf(f, "PlasmaSlowdown %d \n", &Marshmallow_PlasmaSlowdown);

	fscanf(f, "NerfHP_LostSoul %d \n", &Marshmallow_NerfHP_LostSoul);
	fscanf(f, "NerfHP_Cacodemon %d \n", &Marshmallow_NerfHP_Cacodemon);
	fscanf(f, "NerfHP_Demons %d \n", &Marshmallow_NerfHP_Demons);
	fscanf(f, "NerfPC_Archvile %d \n", &Marshmallow_NerfPC_Archvile);  
	fscanf(f, "NerfSpeed_Undead %d \n", &Marshmallow_NerfSpeed_Undead);
	fscanf(f, "FixChainguySound %d \n", &Marshmallow_FixChainguySound);
	fscanf(f, "NerfSkelMissile %d \n", &Marshmallow_NerfSkelMissile);
	fscanf(f, "AltPainDeath %d \n", &Marshmallow_AltPainDeath);
	fscanf(f, "VileZScopeLimit %d \n", &Marshmallow_VileZScopeLimit);
	fscanf(f, "CacoMeleeSound %d \n", &Marshmallow_CacoMeleeSound);

	fscanf(f, "ExtendedMessages %d \n", &Marshmallow_ExtendedMessages);
	fscanf(f, "DeathMessages %d \n", &Marshmallow_DeathMessages);
	fscanf(f, "DamageMessages %d \n", &Marshmallow_DamageMessages);
	fscanf(f, "BerserkReminder %d \n", &Marshmallow_BerserkReminder);  
	fscanf(f, "ExtraTextLines %d \n", &Marshmallow_ExtraTextLines);
	fscanf(f, "PickupMessages %d \n", &Marshmallow_PickupMessages);
	fscanf(f, "BossAlert %d \n", &Marshmallow_BossAlert);
	fscanf(f, "InfightAlert %d \n", &Marshmallow_InfightAlert);
	fscanf(f, "MissileAlert %d \n", &Marshmallow_MissileAlert);
	fscanf(f, "ShowPKEWidget %d \n", &hellscape_on);  
	
	fscanf(f, "BerserkAutoSwitch %d \n", &Marshmallow_BerserkAutoSwitch);
	fscanf(f, "BFGBlastWave %d \n", &Marshmallow_BFGBlastWave);

	if (fscanf(f, "EngineKillingGibs %d \n", &Marshmallow_GibMode))
		Marshmallow_GibMode = DUKE_GIBS;  // If coming from an older version, default to our new Duke3D gib mode
	else
		fscanf(f, "GibMode %d \n", &Marshmallow_GibMode);  // New versions get the gib mode they have saved

	fscanf(f, "berserk_redscreen_length %d \n", &berserk_redscreen_length);
	fscanf(f, "ShowTargetHP %d \n", &Marshmallow_ShowTargetHP);
	fscanf(f, "TreasureMode %d \n", &Marshmallow_TreasureMode);
	fscanf(f, "FastMusicChanges %d \n", &Marshmallow_FastMusicChanges);

	fscanf(f, "DrawWallpaper %d \n", &Marshmallow_DrawWallpaper);
	fscanf(f, "dm_fraglimit %d \n", &dm_fraglimit);
	fscanf(f, "dm_timelimit %d \n", &dm_timelimit);
	fscanf(f, "physics_mode %d \n", &physics_mode);
	fscanf(f, "KeepFreeAmmo %d \n", &Marshmallow_KeepFreeAmmo);
	fscanf(f, "SSG_Level %d \n", &SSG_Level);
	fscanf(f, "AlternateLighting %d \n", &Marshmallow_AlternateLighting);
	fscanf(f, "ResizeMonsters %d \n", &Marshmallow_ResizeMonsters);
	fscanf(f, "Flashlight %d \n", &Marshmallow_Flashlight);
	fscanf(f, "ShowBotStates %d \n", &ShowBotStates);
	//fscanf(f, "default_bot_speed %d \n", &default_bot_speed);
	fscanf(f, "TrueRandomDamage %d \n", &Marshmallow_TrueRandomDamage);
	fscanf(f, "EpicBossDeaths %d \n", &Marshmallow_EpicBossDeaths);
	fscanf(f, "barrel_fx %d \n", &barrel_fx);

	fscanf(f, "F_Key_BotFollow %d \n", &F_Key_BotFollow);
	fscanf(f, "AllowSprint %d \n", &Marshmallow_AllowSprint);

	fscanf(f, "BarrelPushing %d \n", &Marshmallow_BarrelPushing);

	fscanf(f, "DangerousBFG %d \n", &Marshmallow_DangerousBFG);

	fscanf(f, "DM_AllowExit %d \n", &Marshmallow_AllowExit);
	fscanf(f, "DM_KillOnExit %d \n", &Marshmallow_KillOnExit);

	fclose(f);
}


void CheckForCfgFile()
{
	FILE *cfg_file;

	cfg_file = fopen(FILENAME, "r");

	// If no .cfg file exists yet, create one and write default settings 
	if (!cfg_file)
	{
		WritePreferences();
		return;
	}

	// Load saved preferences from marshmallow.cfg
	LoadPreferences(cfg_file);
}


void WritePreferences_OnExit() 
{
	if (realnetgame || M_CheckParm("-solo-net"))
		return;

	WritePreferences(); 
}