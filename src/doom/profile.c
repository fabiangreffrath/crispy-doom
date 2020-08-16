////////////////////////////////////////////
//
//  PROFILE.C:  Player profile, stats, etc.
//
////////////////////////////////////////////

#include "marshmallow.h"

extern void CrispyReplaceColor (char *str, const int cr, const char *col);
extern char* net_player_name;

static void ReadProfile()
{
	FILE *f;
	f = fopen(PROFILE, "r");

	if (!f)
	{
		SHOW_MESSAGE "COULD NOT OPEN PROFILE.CFG!";
		return;
	}

	if ( fscanf(f, "ProfileName %s \n", profile.player_name) )
		SHOW_MESSAGE "OLD PROFILE FORMAT DETECTED.";
	
	fscanf(f, "MonstersKilled %d \n", &profile.stats.monsters_killed);
	fscanf(f, "Accuracy %d \n", &profile.stats.accuracy);
	fscanf(f, "ShotsFired %d \n", &profile.stats.shots_fired);
	fscanf(f, "ShotsHit %d \n", &profile.stats.shots_hit);

	fscanf(f, "Levels %d \n", &profile.stats.levels_completed);
	fscanf(f, "Episodes %d \n", &profile.stats.episodes_completed);
	fscanf(f, "Games %d \n", &profile.stats.games_completed);
	fscanf(f, "Bosses %d \n", &profile.stats.bosses_killed);
	fscanf(f, "Secrets %d \n", &profile.stats.secrets_found);

	fscanf(f, "AllTreasure %d \n", &profile.stats.alltreasure);
	fscanf(f, "AllKills %d \n", &profile.stats.allkills);

	fscanf(f, "SandboxKills %d \n", &profile.stats.sandbox_kills);
	fscanf(f, "SandboxWins %d \n", &profile.stats.sandbox_games_won);
	fscanf(f, "SandboxBosses %d \n", &profile.stats.sandbox_bosses_killed);

	fscanf(f, "DMFrags %d \n", &profile.stats.dm_frags);
	fscanf(f, "KDRatio %d \n", &profile.stats.kd_ratio);
	fscanf(f, "Teamkills %d \n", &profile.stats.players_teamkilled);

	fscanf(f, "TreasureScore %d \n", &profile.treasure_bank.score);  

	fscanf(f, "Treasure_IceSkulls %d \n", &profile.treasure_bank.blueskulls);  
	fscanf(f, "Treasure_GoldSkulls %d \n", &profile.treasure_bank.goldskulls);  
	fscanf(f, "Treasure_BloodSkulls %d \n", &profile.treasure_bank.redskulls);  

	fclose(f);
}


void SaveStats()  // rename to WriteProfile  or SaveProfile()   // TODO:  += existing nums after writeprofile()
{
	FILE *f;

	if ( we_got_a_cheater )
	{
		SHOW_MESSAGE "YOU CHEATED! NO STATS FOR YOU!";
		return;
	}
	
	GetProfile();

	f = fopen(PROFILE, "w");

	if (!f)
	{
		SHOW_MESSAGE "COULD NOT OPEN PROFILE.CFG!";
		return;
	}

	//fprintf(f, "ProfileName %s \n", net_player_name);  

	fprintf(f, "MonstersKilled %d \n", level_stats.monsters_killed + profile.stats.monsters_killed); 
	fprintf(f, "Accuracy %d \n", CalculateAccuracy( level_stats.shots_fired + profile.stats.shots_fired, level_stats.shots_hit + profile.stats.shots_hit ));  // average both accuracy scores together
	fprintf(f, "ShotsFired %d \n", level_stats.shots_fired + profile.stats.shots_fired);
	fprintf(f, "ShotsHit %d \n", level_stats.shots_hit + profile.stats.shots_hit);

	fprintf(f, "Levels %d \n", level_stats.levels_completed + profile.stats.levels_completed);
	fprintf(f, "Episodes %d \n", level_stats.episodes_completed + profile.stats.episodes_completed);
	fprintf(f, "Games %d \n", level_stats.games_completed + profile.stats.games_completed);
	fprintf(f, "Bosses %d \n", level_stats.bosses_killed + profile.stats.bosses_killed);
	fprintf(f, "Secrets %d \n", level_stats.secrets_found + profile.stats.secrets_found);

	fprintf(f, "AllTreasure %d \n", level_stats.alltreasure + profile.stats.alltreasure);
	fprintf(f, "AllKills %d \n", level_stats.allkills + profile.stats.allkills);

	fprintf(f, "SandboxKills %d \n", saved_sandbox.count + profile.stats.sandbox_kills);  // NOTE: using sandbox monster count here
	fprintf(f, "SandboxWins %d \n", level_stats.sandbox_games_won + profile.stats.sandbox_games_won);
	fprintf(f, "SandboxBosses %d \n", level_stats.sandbox_bosses_killed + profile.stats.sandbox_bosses_killed);

	fprintf(f, "DMFrags %d \n", level_stats.dm_frags + profile.stats.dm_frags);
	fprintf(f, "KDRatio %d \n", level_stats.kd_ratio);
	fprintf(f, "Teamkills %d \n", level_stats.players_teamkilled);

	fprintf(f, "TreasureScore %d \n", treasure_bag.score + profile.treasure_bank.score); 

	fprintf(f, "Treasure_IceSkulls %d \n", treasure_bag.blueskulls + profile.treasure_bank.blueskulls);  
	fprintf(f, "Treasure_GoldSkulls %d \n", treasure_bag.goldskulls + profile.treasure_bank.goldskulls);  
	fprintf(f, "Treasure_BloodSkulls %d \n", treasure_bag.redskulls + profile.treasure_bank.redskulls);  

	fclose(f);

	SHOW_MESSAGE DEH_String(STATSOK);
}



boolean GetProfile()  
{
	FILE *f;
	f = fopen(PROFILE, "r");

	if (!f)
	{
		SHOW_MESSAGE DEH_String(STATSHINT);
		return false; 
	}

	fclose(f);  // since we re-open it below

	ReadProfile();  // what the hell do we need to read anyway?  let's just do += score on exit

	return true;
}



// Called when any cheat or level skipping occurs
void DisableStats()
{
    // Only come in here the first time
	if ( we_got_a_cheater )
		return;

	we_got_a_cheater = true;

	SHOW_MESSAGE "CHEATING BASTARD! NO STATS FOR YOU!";
}

void ResetStats()  
{
	we_got_a_cheater = false;
}


int CalculateAccuracy(int shots, int hit)
{
	float f_acc; 
	int acc;

    // Only calculate if both are above zero
	if (shots && hit)
	{
		f_acc = (float)hit / (float)shots; 
		acc = f_acc * 100;
	}
	else
	{
		acc = 0;
	}

	return acc;
}


void PrepareProfileData()
{
	int kills = profile.stats.monsters_killed + level_stats.monsters_killed;
	int frags = profile.stats.dm_frags + level_stats.dm_frags; // NEW
	int shots = profile.stats.shots_fired; 
	int hit = profile.stats.shots_hit;
	int levels = profile.stats.levels_completed; 
	int epi = profile.stats.episodes_completed + level_stats.episodes_completed;
	int games = profile.stats.games_completed + level_stats.games_completed;
	int bosses = profile.stats.bosses_killed + level_stats.bosses_killed;
	int secrets = profile.stats.secrets_found + level_stats.secrets_found;  // was MAIN_PLAYER.secretcount
	int s_kills = profile.stats.sandbox_kills + level_stats.sandbox_kills;
	int s_won = profile.stats.sandbox_games_won + level_stats.sandbox_games_won;
	int tr_s = profile.treasure_bank.score + treasure_bag.score;
	int tr_b = profile.treasure_bank.blueskulls + treasure_bag.blueskulls;
	int tr_g = profile.treasure_bank.goldskulls + treasure_bag.goldskulls;
	int tr_r = profile.treasure_bank.redskulls + treasure_bag.redskulls;
	int allt = profile.stats.alltreasure + level_stats.alltreasure;
	int allk = profile.stats.allkills + level_stats.allkills;
	char* name = net_player_name; // was profile.player_name;
	char buf[16];

	int acc = CalculateAccuracy(shots, hit);

	menus_on = true;  // HACK: so we can use keyboard

	if (strlen(name) < 12) 
		CrispyReplaceColor(name, CR_GOLD, name);
	HUlib_addMessageToSText(&profile_namedisplay, DEH_String("  Player Name:  "), DEH_String(name) );


	// TESTING: Set up DMFrags tracking in profile so we can report them on the profile while playing DM
	//
	if ( !deathmatch ) // coop
	{
	sprintf(buf, "%d", kills);
	if (kills > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_monsters_killed, "       Monsters Killed:  ", DEH_String(buf) );
	}
	else // deathmatch
	{
	sprintf(buf, "%d", frags);
	if (frags > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_monsters_killed, "       Deathmatch Frags:  ", DEH_String(buf) );	
	}

	sprintf(buf, "%d", acc);
	if (acc > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_accuracy, "       Accuracy:  ", DEH_String(buf) );

	sprintf(buf, "%%");
	CrispyReplaceColor(buf, CR_GREEN, buf);
	HUlib_addMessageToSText(&profile_percent, NULL, DEH_String(buf) );

	sprintf(buf, "%d", allt);
	if (allt > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_alltreasure, "       100% Treasure:  ", DEH_String(buf) );

	sprintf(buf, "%d", allk);
	if (allk > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_allkills, "       100% Kills:  ", DEH_String(buf) );

	sprintf(buf, "%d", levels);
	if (levels > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_levels_completed, "       Levels Completed:  ", DEH_String(buf) );

	sprintf(buf, "%d", epi);
	if (epi > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_episodes_completed, "       Episodes Completed:  ", DEH_String(buf) );

	sprintf(buf, "%d", games);
	if (games > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_games_completed, "       Games Completed:  ", DEH_String(buf) );

	sprintf(buf, "%d", bosses);
	if (bosses > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_bosses_killed, "       Bosses Killed:  ", DEH_String(buf) );
	
	sprintf(buf, "%d", secrets);
	if (secrets > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_secrets_found, "       Secrets Found:  ", DEH_String(buf) );

	sprintf(buf, "%d", s_kills);
	if (s_kills > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_sandbox_kills, "       Sandbox Kills:  ", DEH_String(buf) );

	sprintf(buf, "%d", s_won);
	if (s_won > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_sandbox_games_won, "       Sandbox Games Won:  ", DEH_String(buf) );

	sprintf(buf, "%d", tr_s);
	if (tr_s > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_treasurescore, "       Treasure Points:  ", DEH_String(buf) );

	sprintf(buf, "%d", tr_b);
	if (tr_b > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_treasureblue, "       Ice Skulls Collected:  ", DEH_String(buf) );

	sprintf(buf, "%d", tr_g);
	if (tr_g > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_treasuregold, "       Gold Skulls Collected:  ", DEH_String(buf) );

	sprintf(buf, "%d", tr_r);
	if (tr_r > 0) { CrispyReplaceColor(buf, CR_GREEN, buf); }
	HUlib_addMessageToSText(&profile_treasurered, "       Blood Skulls Collected:  ", DEH_String(buf) );
}


void DrawProfile()  
{
	if ( GetGameType() != DOOM2 )
		Draw_Wallpaper(MENU_WALLPAPER);  

	HUlib_drawSText(&profile_namedisplay);
	
	HUlib_drawSText(&profile_accuracy);
	HUlib_drawSText(&profile_percent);

	HUlib_drawSText(&profile_alltreasure);
	HUlib_drawSText(&profile_allkills);
		
	HUlib_drawSText(&profile_monsters_killed);
	HUlib_drawSText(&profile_bosses_killed);
	HUlib_drawSText(&profile_secrets_found);
	HUlib_drawSText(&profile_sandbox_kills);
	HUlib_drawSText(&profile_sandbox_games_won);
	HUlib_drawSText(&profile_levels_completed);

	HUlib_drawSText(&profile_treasurescore);
	HUlib_drawSText(&profile_treasureblue);
	HUlib_drawSText(&profile_treasuregold);
	HUlib_drawSText(&profile_treasurered);
}


boolean LaunchProfileScreen()
{
	if ( GetProfile() )
	{
		PrepareProfileData();
		return true;
	}

	return false;
}