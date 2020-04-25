/////////////////////////////////////////////////////
//
//  MARSHMALLOW.C:  Entry point, init settings, etc.
//
/////////////////////////////////////////////////////

#include "marshmallow.h"

void Marshmallow_InitVariables()
{	
	PKE_Activate();

	F_Key_BotFollow = true;
	Marshmallow_AllowSprint = true;
	
	cursor_x = CURSOR_X_DATAPAD;

	SetCrispyOptions();

	Marshmallow_AllowExit = false;
	Marshmallow_KillOnExit = true;

	Marshmallow_BarrelPushing = true;

	Marshmallow_PlayerCollision = true; 

	Marshmallow_GibMode = DUKE_GIBS;   

	Marshmallow_TrueInvisibility = true; 

	botcursor_on = false;   

	newgame_mode = SINGLEPLAYER;

	Doom_DJ.song_length = LONG_SONG_LENGTH;  

	newskill = skill_selection = HMP_SELECTED;

	upgrade_chance = 0;
	MonsterHitpointsScale = 1;  // 1x means default spawnhealth

	Marshmallow_ConsistencyChk = true;  

	Marshmallow_SelfDamage = true; 
	Marshmallow_EndMapNowCheat = true;  
	Marshmallow_SuicideCheat = true;   
	Marshmallow_LootMultiplier = 1;
	Marshmallow_BerserkReminder = true;  
	Marshmallow_BossAlert = true;
	sprint_timeout = DEFAULT_SPRINT_TIMEOUT;
	sprint_recharge = DEFAULT_SPRINT_RECHARGE;
	SetWaypointIcon(HIDE);  
	Marshmallow_ShowTargetHP = 2; // By default, show target HP for bosses only
	skip_to_level = 0;  // "RANDOM" level will be shown first on level selection line
	berserk_redscreen_length = BERSERK_REDSCREEN_SHORT;

	hellscape_on = true;
	Marshmallow_MissileAlert = true;
	Marshmallow_RespawnInNightmare = true;

	Marshmallow_DeathMessages = true;
	Marshmallow_PickupMessages = false;  // Start with item pickup messages off since we have plenty of new messages spamming the console
	Marshmallow_ConservePowerups = true; 
	Marshmallow_GradedWeapons = true;

	Marshmallow_PlasmaSlowdown = true;  // We'll try this on by default for now
	Marshmallow_BFGBlastWave = true;
	Marshmallow_DangerousBFG = true;

	Marshmallow_SaveItems = true; 

	//if ( !Marshmallow_PlayingSigil ) 
		Marshmallow_DynamicMusic = true;  

	// Weapon tweaks will start out turned off:
	BoostChainsaw(OFF);
	BoostFist(OFF);
	BoostBullets(OFF);
	InfinitePistol(OFF);
	BalanceShotguns(OFF);
	SetPunchSound();

	// Default monster tweaks:
	NerfHP_LostSoul(ON);
	NerfHP_Cacodemon(OFF);
	NerfHP_Demons(OFF);
	
	NerfSpeed_Undead(ON);
	NerfSkelMissile(ON);
	ChangePainDeath(ON);
	NerfPC_Archvile(ON);	
	FixChainguySound(ON);

	vile_damage = VANILLA_VILE_DAMAGE;  // never made an option for this, so set to default
	Marshmallow_VileZScopeLimit = true; 

	e3m8_caco_upgrade = false;

	Doom1SSG_Level = SSG_Level = DEFAULT_SSG_LEVEL;

	physics_mode = 0;  

	Marshmallow_DropBackpack = true;  // We need this on for our dropped items backpack

	dm_timelimit = 10;
	dm_fraglimit = 50;

	Marshmallow_PlasmaSlowdown = true;

	Marshmallow_KeepFreeAmmo = true;   // WIP

	Marshmallow_ResizeMonsters = true;

	Marshmallow_Flashlight = true;

	specialnodes = false;
	barrel_fx = true;

	Marshmallow_TrueRandomDamage = false;  // off by default for now

	ShowBotStates = true;  // new
	default_bot_speed = BOTS_WALK;  // new

	botstate[BOT_1] = " ";
	botstate[BOT_2] = " ";
	botstate[BOT_3] = " ";

	Marshmallow_EpicBossDeaths = true;

	Marshmallow_KeepKeys = true;   
	Marshmallow_KeepWeapons = true; 
	Marshmallow_WeaponsStay = true;  // test

	if ( !realnetgame && !M_CheckParm("-solo-net") )
	{
		// Load preferences from disk
		CheckForCfgFile();  

		// Toggle game tweaks that we just loaded from cfg
		SetOptionsFromCfg();
	}
	else  
	{
		// Defaults for real network games

		Marshmallow_SaveItems = false;
		Marshmallow_DynamicMusic = false;  // dynamic music is still giving us consistency crashes
		Marshmallow_PlasmaSlowdown = false;  // firingdelay would need to be player-specific for this to work
		Marshmallow_GiftDropping = true;  

		if ( M_CheckParm("-server") || M_CheckParm("-solo-net"))  // trying this here instead of d_net.c
			CheckForServerCfgFile();
	}

	// Change the invis sprite to appear with MF_SHADOW effect
	if (Marshmallow_DropGoodies)
		ChangeInvis(ON);
}


void Marshmallow_CheckCommandLineArgs() 
{
	int p, n;

	// for upgrades we take an input of 1-9
	if ((p = M_CheckParmWithArgs("-upgrade", 1))
		|| (p = M_CheckParmWithArgs("-ug", 1)))   
	{
		n = myargv[p+1][0]-'0';

		upgrade_chance = n*10;

		if ( upgrade_chance < 10 || upgrade_chance > 90 )  // if we get junk input, default to 50
			upgrade_chance = 50; 
	}
	//else if (M_CheckParm("-upgrade"))   // if we didn't get a number, default to 100
	//{
	//	upgrade_chance = 100;
	//}
	else   // otherwise, upgrading is disabled
	{
		upgrade_chance = NULL;
	}

	if (p = M_CheckParmWithArgs("-timelimit", 1))
	{
		n = myargv[p+1][0]-'0';

		dm_timelimit = n*MINUTE;
	}

	if (p = M_CheckParmWithArgs("-fraglimit", 1))
	{
		n = myargv[p+1][0]-'0';

		dm_fraglimit = n;
	}

	if (M_CheckParm("-ri"))  
		Marshmallow_RandomItems = 1;

	if (M_CheckParm("-rie"))  
		Marshmallow_RandomItems = 2;

	if (M_CheckParm("-debug"))
		debugmode = true;

	//if (M_CheckParm("-cl"))
	//	Marshmallow_ColoredLighting = true;

	//if (M_CheckParm("-vpc"))
	//	mobjinfo[MT_VILE].painchance = 100;  

	//if (M_CheckParm("-spc"))
	//	mobjinfo[MT_SPIDER].painchance = 100;  

	//if (M_CheckParm ("-ekg"))
	//	Marshmallow_GibMode = true;

	if (p = M_CheckParmWithArgs("-gibs", 1))
	{
		n = myargv[p+1][0]-'0';

		switch (n)
		{
		case NO_GIBS:
			Marshmallow_GibMode = NO_GIBS;
			break;
		case ROTT_GIBS:
			Marshmallow_GibMode = ROTT_GIBS;
			break;
		case DUKE_GIBS:
			Marshmallow_GibMode = DUKE_GIBS;
			break;
		case BRUTAL_GIBS:
			Marshmallow_GibMode = BRUTAL_GIBS;
			break;
		default:
			Marshmallow_GibMode = DUKE_GIBS;
		}
	}

	if (M_CheckParm ("-info"))
		Marshmallow_InfoReadout = true;

	if (M_CheckParm ("-show"))
		SetWaypointIcon(SHOW);

	if (M_CheckParm ("-sandbox"))
	{
		autostart = true;
		InitSandbox();
	}
	
	//if (M_CheckParm ("-dj"))
	//{
	//	Marshmallow_DynamicMusic = true;
	//	Doom_DJ.autostart = true;
	//}

	if (M_CheckParm ("-ps"))  // changed from -gw; we're leaving graded weapons on by default, and we can disable it with -ps (short for pistol start)
		Marshmallow_GradedWeapons = false;

	//if (M_CheckParm ("-keepitems"))  // TODO: this is on by default now; change this to a disable option instead
	//	Marshmallow_SaveItems = true;

	// VERIFY: do we need autostart = true in these too?

	if (M_CheckParm ("-itytd"))
	{
		autostart = true;
		startskill = sk_baby;
	}

	if (M_CheckParm ("-hntr"))
	{
		autostart = true;
		startskill = sk_easy;
	}

	if (M_CheckParm ("-hmp"))
	{
		autostart = true;
		startskill = sk_medium;
	}
	
	if (M_CheckParm ("-uv"))
	{
		autostart = true;
		startskill = sk_hard;
	}

	if (M_CheckParm ("-nm"))
	{
		autostart = true;
		startskill = sk_nightmare;
	}

	if (M_CheckParm ("-nm2"))
	{
		autostart = true;  // so it functions like -skill 
		Marshmallow_InitAltNightmare();
	}

	if (M_CheckParm ("-nmnr"))
	{
		autostart = true;
		startskill = sk_nightmare;
		Marshmallow_RespawnInNightmare = false; 
	}

	if (M_CheckParm ("-uv2"))
	{
		autostart = true;
		startskill = sk_hard;
		Marshmallow_InitAltUltraViolence();
	}
   
	if (M_CheckParm ("-itemrespawn"))  // both coop and singleplayer?
		Marshmallow_CoopItemRespawn = true;

	//if (M_CheckParm ("-goodies"))  // TODO: this is on by default now, so make this -nogoodies instead?
	//	Marshmallow_DropGoodies = true; // TODO: if using nogoodies, explicitly set to false

	//if (M_CheckParm ("-conserve"))  // both coop and singleplayer?   // TODO: this is on by default now, so make this -noconserve instead?
	//	Marshmallow_ConservePowerups = true;

	if (M_CheckParm ("-ff"))  // TODO: decide if these should explicitly set true or false
		Marshmallow_FriendlyFire = true;

	if (M_CheckParm ("-mirror"))  // TODO: decide if these should explicitly set true or false
		Marshmallow_MirrorDamage = true;

	if (M_CheckParm ("-self"))  // TODO: decide if these should explicitly set true or false
		Marshmallow_SelfDamage = true;

	//if (M_CheckParm ("-keepguns"))  // TODO: this is on by default now, so make this -nokeepguns instead?
	//	Marshmallow_KeepWeapons = true;

	//if (M_CheckParm ("-keepkeys"))   // TODO: this is on by default now, so make this -nokeepkeys instead?
	//	Marshmallow_KeepKeys = true;

	if (M_CheckParm("-delayssg"))   // if no level number is specified, default to MAP04
	{
		SSG_Level = DEFAULT_SSG_LEVEL;  
			
		Marshmallow_WitholdSSG = true;
	}
	
	if ((p = M_CheckParmWithArgs("-delayssg", 1)))
	{
		SSG_Level = myargv[p+1][0]-'0';

		if (SSG_Level < 1)							
			SSG_Level = DEFAULT_SSG_LEVEL;  
			
		Marshmallow_WitholdSSG = true;
	}

	if ((p = M_CheckParmWithArgs("-d1ssg", 1)))  // NOTE: probably pointless since Crispy already does this
	{
		// TODO: act like -delayssg above
		// we can just change Marshmallow_Doom1SSG to int type and use that

		Marshmallow_Doom1SSG = true;
	}
	
	if (M_CheckParm("-bot")
		|| M_CheckParm("-bots"))
		{
			netgame = true;   
		}

	if (p = M_CheckParmWithArgs("-scale", 1))
		MonsterHitpointsScale = myargv[p+1][0]-'0';

	if (M_CheckParm ("-dmw"))   
		Marshmallow_DeathmatchWeapons = true;

	if (M_CheckParm ("-dmm"))   // NEW
		nomonsters = false;

	if (M_CheckParm ("-tr"))
		Marshmallow_TreasureMode = 1;

	if (M_CheckParm ("-trb"))
		Marshmallow_TreasureMode = 2;

	if (M_CheckParm ("-rand"))   
	{
		startmap = RandomMap();
		startepisode = RandomEpisode();

		autostart = true;

		crispy->demowarp = startmap;
	}

	//if (M_CheckParm ("-djmsg"))
	//	Marshmallow_DJMessages = true;

	//if (M_CheckParm("-d3l"))
	//	Marshmallow_AlternateLighting = D3_LIGHTING;
	//else if (M_CheckParm("-crt"))
	//	Marshmallow_AlternateLighting = CRT_LIGHTING;

	if (M_CheckParm("-clip"))
		Marshmallow_PlayerCollision = false;

	if (M_CheckParm("-flip"))  // NEW
		crispy->fliplevels = !crispy->fliplevels;

	if (deathmatch) 
	{
		Marshmallow_DeathmatchWeapons = true;

		if (!M_CheckParm("-dmm"))
		nomonsters = true;  

		autostart = true; 
	}
}


void SetCrispyOptions()   
{
	showMessages = 1;  // we always need showMessages on 
	
	if (crispy->coloredhud == COLOREDHUD_OFF)
		crispy->coloredhud = COLOREDHUD_TEXT;  // Crispy's colored HUD text is required for our new in-game menus

	if (crispy->coloredhud == COLOREDHUD_BAR)
		crispy->coloredhud = COLOREDHUD_BOTH;

	//crispy->uncapped = UNCAPPED_VSYNC;	
	
	//crispy->coloredblood = COLOREDBLOOD_BOTH;
	crispy->brightmaps = BRIGHTMAPS_BOTH;
	crispy->translucency = TRANSLUCENCY_BOTH;
	crispy->flipcorpses = true;
	crispy->overunder = true;
}


cheatseq_t Marshmallow_PhysicsCheatString = CHEAT(MARSHMALLOW_CHEAT_PHYSICS, 0);
cheatseq_t Marshmallow_FlipLevelsCheatString = CHEAT(MARSHMALLOW_CHEAT_FLIPLEVELS, 0);
cheatseq_t Marshmallow_BotMove = CHEAT(MARSHMALLOW_CHEAT_BOTMOVE, 0); 
cheatseq_t Marshmallow_BotWait = CHEAT(MARSHMALLOW_CHEAT_BOTWAIT, 0); 
cheatseq_t Marshmallow_FriendlyFireCheatString = CHEAT(MARSHMALLOW_CHEAT_FRIENDLYFIRE, 0); 
cheatseq_t Marshmallow_GamemodeCheatString = CHEAT(MARSHMALLOW_CHEAT_GAMEMODE, 0); 
cheatseq_t Marshmallow_ShowInfoCheatString = CHEAT(MARSHMALLOW_CHEAT_SHOWINFO, 0); 
cheatseq_t Marshmallow_EKGCheatString = CHEAT(MARSHMALLOW_CHEAT_EKG, 0); 
cheatseq_t Marshmallow_AllItemsCheatString = CHEAT(MARSHMALLOW_CHEAT_ALLITEMS, 0); 
cheatseq_t Marshmallow_DipstickCheatString = CHEAT(MARSHMALLOW_CHEAT_DIPSTICK, 0); 
cheatseq_t Marshmallow_EndMapCheatString = CHEAT(MARSHMALLOW_CHEAT_ENDMAPNOW, 0); 
cheatseq_t Marshmallow_SuicideCheatString = CHEAT(MARSHMALLOW_CHEAT_KILLMENOW, 0); 
cheatseq_t Marshmallow_RestartMapCheatString = CHEAT(MARSHMALLOW_CHEAT_RESTARTMAP, 0); 
cheatseq_t Marshmallow_AutoUseCheatString = CHEAT(MARSHMALLOW_CHEAT_AUTOUSE, 0); 
cheatseq_t Marshmallow_FastMonstersCheatString = CHEAT(MARSHMALLOW_CHEAT_FASTMONSTERS, 0); 
cheatseq_t Marshmallow_CheckSkillCheatString = CHEAT(MARSHMALLOW_CHEAT_CHECKSKILL, 0); 
cheatseq_t Marshmallow_ChangeSkill1CheatString = CHEAT(MARSHMALLOW_CHEAT_CHANGESKILL1, 0); 
cheatseq_t Marshmallow_ChangeSkill2CheatString = CHEAT(MARSHMALLOW_CHEAT_CHANGESKILL2, 0); 
cheatseq_t Marshmallow_ChangeSkill3CheatString = CHEAT(MARSHMALLOW_CHEAT_CHANGESKILL3, 0); 
cheatseq_t Marshmallow_ChangeSkill4CheatString = CHEAT(MARSHMALLOW_CHEAT_CHANGESKILL4, 0); 
cheatseq_t Marshmallow_ChangeSkill5CheatString = CHEAT(MARSHMALLOW_CHEAT_CHANGESKILL5, 0); 
cheatseq_t Marshmallow_ChangeSkill6CheatString = CHEAT(MARSHMALLOW_CHEAT_CHANGESKILL6, 0); 
cheatseq_t Marshmallow_NoRespawnCheatString = CHEAT(MARSHMALLOW_CHEAT_NORESPAWN, 0); 
cheatseq_t Marshmallow_NextTrackCheatString = CHEAT(MARSHMALLOW_CHEAT_NEXTTRACK, 0); 


void Marshmallow_CheckCheats(event_t* ev)
{
	if (cht_CheckCheat(&Marshmallow_NextTrackCheatString, ev->data2) && !realnetgame)
	{
		if (Marshmallow_DynamicMusic)
		{
			DJ_NextTrack();
		}
		else  // Vanilla music mode:
		{
			int newsong = GetReplacementSong();
			S_ChangeMusic(newsong, true);  
			SHOW_MESSAGE "SONG SKIPPED.";
		}
	}

	if (cht_CheckCheat(&Marshmallow_NoRespawnCheatString, ev->data2) && !realnetgame)
	{
		if (gameskill != sk_nightmare)
			return;

		if (!Marshmallow_RespawnInNightmare)
		{
			Marshmallow_RespawnInNightmare = true;
			SHOW_MESSAGE "MONSTER RESPAWNING ENABLED.";
		}
		else
		{
			Marshmallow_RespawnInNightmare = false;
			SHOW_MESSAGE "MONSTER RESPAWNING DISABLED.";
		}

		HideAllMenus();  // doesn't work here
	}

	if (cht_CheckCheat(&Marshmallow_PhysicsCheatString, ev->data2) && !realnetgame)
	{
		if ( physics_mode < 2)
		{
			physics_mode = 2;
			SHOW_MESSAGE DEH_String(PHYSON);
		}
		else
		{
			physics_mode = false;
			SHOW_MESSAGE DEH_String(PHYSOFF);
		}

		return;
	}

	if (cht_CheckCheat(&Marshmallow_FlipLevelsCheatString, ev->data2))
	{
		if (crispy->fliplevels)
		{
			crispy->fliplevels = false;
			SHOW_MESSAGE DEH_String(CHANGEFFECT);
		}
		else
		{
			crispy->fliplevels = true;
			SHOW_MESSAGE DEH_String(CHANGEFFECT);
		}

		HideAllMenus();  // doesn't work here
	}

	if (cht_CheckCheat(&Marshmallow_FastMonstersCheatString, ev->data2) && !realnetgame)
	{
		HideAllMenus();
		ToggleFastMonsters();
	}
	
	if (cht_CheckCheat(&Marshmallow_BotMove, ev->data2) && !realnetgame)
	{
		bot_t bot;

		for (bot=BOT_1; bot<=BotsInGame; bot++)
		{
			Bot_StartPatrolling(bot);

			HideAllMenus();  // since the 'M' and 'V' launch menus
		}

		Bot_PlayRadioNoise();
		SHOW_MESSAGE DEH_String(ORDERMSGPATROL);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_BotWait, ev->data2) && !realnetgame)
	{
		Bot_PlayRadioNoise();
		SHOW_MESSAGE DEH_String(ORDERMSGHOLD);

		Bot_HoldPosition();

		HideAllMenus(); 

		return;
	}

	if (cht_CheckCheat(&Marshmallow_RestartMapCheatString, ev->data2))
	{
		if (realnetgame)   // at least until we add a CmdEvent for it to work in netgame
			return;

		RestartMap();
	}

	if (cht_CheckCheat(&Marshmallow_AutoUseCheatString, ev->data2))
	{
		if (realnetgame)   // at least until we get this working in realnetgame
			return;

		if (Marshmallow_AutoUse)
		{
			Marshmallow_AutoUse = false;
			//SHOW_MESSAGE DEH_String();
		}
		else if (!Marshmallow_AutoUse)
		{
			Marshmallow_AutoUse = true;
			//SHOW_MESSAGE DEH_String();
		}

		return;
	}

	if (cht_CheckCheat(&Marshmallow_CheckSkillCheatString, ev->data2))
	{
		CheckCurrentSkill();
	}

	if (cht_CheckCheat(&Marshmallow_ChangeSkill1CheatString, ev->data2)
		&& !realnetgame)
	{
		gameskill = sk_baby;
		SHOW_MESSAGE DEH_String(SKILL1);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_ChangeSkill2CheatString, ev->data2)
		&& !realnetgame)
	{
		gameskill = sk_easy;
		SHOW_MESSAGE DEH_String(SKILL2);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_ChangeSkill3CheatString, ev->data2)
		&& !realnetgame)
	{
		gameskill = sk_medium;
		SHOW_MESSAGE DEH_String(SKILL3);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_ChangeSkill4CheatString, ev->data2)
		&& !realnetgame)
	{
		gameskill = sk_hard;
		SHOW_MESSAGE DEH_String(SKILL4);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_ChangeSkill5CheatString, ev->data2)
		&& !realnetgame)
	{
		gameskill = sk_nightmare;
		SHOW_MESSAGE DEH_String(SKILL5);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_ChangeSkill6CheatString, ev->data2)
		&& !realnetgame)
	{
		if (Marshmallow_AlternateNightmare)
		{
			SHOW_MESSAGE DEH_String(NM2AL);
			// TODO: turn it off
				return;
		}

		SHOW_MESSAGE DEH_String(NM2NEXT);
		Marshmallow_InitAltNightmare();

		return;
	}

	if (Marshmallow_EndMapNowCheat		// Type 'endmap' to exit the level at any time
	  && cht_CheckCheat(&Marshmallow_EndMapCheatString, ev->data2)) 
	{
		if (!realnetgame)
		{
			DisableStats();
			G_ExitLevel();
		}
		else
			Marshmallow_SendMultiplayerEvent(MARSHMALLOW_SOMEONE_ABORTED_MAP);

		SHOW_MESSAGE DEH_String(ABORTMAP);

		return;
	}

	if (Marshmallow_SuicideCheat   // Type 'killmenow' to suicide						   
	  && cht_CheckCheat(&Marshmallow_SuicideCheatString, ev->data2)) 
	{
		if (!realnetgame)
			PlayerKillsHimself(MAIN_PLAYER.mo);
		else
			Marshmallow_SendMultiplayerEvent(MARSHMALLOW_SOMEONE_KILLED_THEMSELF);

		return;
	}

	if (cht_CheckCheat(&Marshmallow_GamemodeCheatString, ev->data2) && !realnetgame)    // TODO: Type 'gamemode' to see (SINGLEPLAYER/NETGAME/REALNETGAME and COOP/DM)
	{
		// TODO ...

		return;
	}

	if (cht_CheckCheat(&Marshmallow_ShowInfoCheatString, ev->data2) && !realnetgame)    // Type 'showinfo' to see our info readout for watching variables in-game
	{
		Marshmallow_InfoReadout = !Marshmallow_InfoReadout;

		HideAllMenus();

		return;
	}

	if (cht_CheckCheat(&Marshmallow_FriendlyFireCheatString, ev->data2) && !realnetgame)    // Type 'friendlyfire' to toggle friendly fire during netgame
	{
		ToggleFriendlyFire();

		return;
	}

	if (cht_CheckCheat(&Marshmallow_EKGCheatString, ev->data2) && !realnetgame)    // Type 'ludicrous' to enable EKG mode for extra blood effects
	{
		return;  // Disabled for now

		if (!Marshmallow_GibMode)
		{
			SHOW_MESSAGE DEH_String(EKGON);
			S_StartSound(NULL, sfx_slop);
			Marshmallow_GibMode = true;
		}
		else
		{
			SHOW_MESSAGE DEH_String(EKGOFF);
			Marshmallow_GibMode = false;
		}

		return;
	}

	if (cht_CheckCheat(&Marshmallow_AllItemsCheatString, ev->data2)    // Type 'giveall' to receive all inventory items
		&& !realnetgame)
	{
		SHOW_MESSAGE DEH_String(GIVEALL);

		HideAllMenus(); // since the 'v' key will accidentally launch the inventory screen

		GiveAllItems();

		DisableStats();

		return;
	}

	if (cht_CheckCheat(&Marshmallow_DipstickCheatString, ev->data2)    // Type 'chojin' for god mode and all weapons/items (like the 'chojin' cheat from Rise of the Triad)
		&& !realnetgame)
	{
		int i, p;
		player_t* plyr = &MAIN_PLAYER;

		plyr->cheats ^= CF_GODMODE;
		
		if ( plyr->cheats & CF_GODMODE )
		{

			// TODO: color shift for a second or two

			if (!plyr->backpack)
			{
				for (i=0 ; i<NUMAMMO ; i++)
				plyr->maxammo[i] *= 2;
				plyr->backpack = true;
			}

			// Give all keys to player and bots
			for (p=0; p<=BotsInGame; p++)   
			{
				player_t* key_recipient = &players[p];

				for (i = 0; i < NUMCARDS; i++)
					key_recipient->cards[i] = true;
			}
		
			for (i=0;i<NUMWEAPONS;i++)
			  plyr->weaponowned[i] = true;
	
			for (i=0;i<NUMAMMO;i++)
			  plyr->ammo[i] = plyr->maxammo[i];

			if (plyr->mo)
			  plyr->mo->health = 100;
	
			plyr->bonuscount += 2;

			plyr->message = DEH_String(WWWCHEAT);

			we_got_a_cheater = true;
    
			GiveAllItems();

			DisableStats();
		}
		else
		{
			plyr->message = DEH_String("GOD MODE DISABLED!");
		}

		HideAllMenus(); 
		return;
	}
}


int CheckIfFileExists(const char *filename, const char *mode)
{
	SDL_RWops* file;
	int		   result;

	file = SDL_RWFromFile( filename, mode );

	if (file)
	{
		result = 1;
		SDL_RWclose( file );
	}
	else
	{
		result = 0;
	}

	return result;
}


int CheckForOtherIWAD()
{
	int result; 

	if (Marshmallow_WadStealing   // Check if we already did wad stealing from the command line
		||  M_CheckParm("-nosteal") )  // skips this prompt
		return 0;

	if (PLAYING_DOOM2)
		result = CheckIfFileExists( "DOOM.WAD", "r+b" );
	else if (PLAYING_DOOM1)
		result = CheckIfFileExists( "DOOM2.WAD", "r+b" );
	else
		result = 0;

	return result;
}


int WadSelection()
{
	int buttonid;

	const SDL_MessageBoxButtonData buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "DOOM2.WAD" },
		{ 0, 0, "DOOM.WAD" },
	};

	const SDL_MessageBoxColorScheme colorScheme = {
		{ 
			{ 255,   0,   0 },
			{   0, 255,   0 },
			{ 255, 255,   0 },
			{   0,   0, 255 },
			{ 255,   0, 255 }
		}
	};

	SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		" ",
		" ", 
		SDL_arraysize(buttons),
		buttons,
		&colorScheme
	};

	messageboxdata.title = "IWAD selection";
	messageboxdata.message = "Please select IWAD for this session:";

	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) 
		return 0;

	if (buttonid == 1) 
		return 1;
	else
		return 0;
}


int AskForWadStealing()
{
	int buttonid;

	const SDL_MessageBoxButtonData buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Yes" },
		{ 0, 0, "No" },
	};

	const SDL_MessageBoxColorScheme colorScheme = {
		{ 
			{ 255,   0,   0 },
			{   0, 255,   0 },
			{ 255, 255,   0 },
			{   0,   0, 255 },
			{ 255,   0, 255 }
		}
	};

	SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		NULL,
		" ",
		" ", 
		SDL_arraysize(buttons),
		buttons,
		&colorScheme
	};

	if (modifiedgame)  // If we're not playing vanilla Doom or Doom II, don't prompt user for Wad Stealing
		return 0;

	if (PLAYING_DOOM2)
	{
		messageboxdata.title = "Marshmallow DOOM II";
		messageboxdata.message = "Would you also like to import music, graphics, and sound from DOOM.WAD for a richer Doom experience?";
	}
	else
	{
		messageboxdata.title = "Marshmallow Ultimate DOOM ";
		messageboxdata.message = "Would you also like to import music, graphics, and sound from DOOM2.WAD for a richer Doom experience?";
	}

	if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) 
		return 0;

	if (buttonid == 1) 
		return 1;
	else
		return 0;
}


void DoWadStealing()
{
    char* filename;
	int   result;

	if (PLAYING_DOOM2)
		filename = "DOOM.WAD";
	else
		filename = "DOOM2.WAD";

	result = CheckIfFileExists(filename, "r+b");

	if ( !result )
		return;   

	Marshmallow_WadStealing = true;
	
	if (PLAYING_DOOM1)
		Marshmallow_Doom1SSG = true;
	
	W_MergeFile(filename);
}

fixed_t save_x, save_y;

static void DistanceCheck()
{
	fixed_t dist;
//	fixed_t x, y;

	if (gametic == 3)
	{
		save_x = MAIN_PLAYER.mo->x;
		save_y = MAIN_PLAYER.mo->y;
	}

	dist = P_AproxDistance( MAIN_PLAYER.mo->x - save_x,
							MAIN_PLAYER.mo->y - save_y );

	if (dist > 64*64*FRACUNIT)
		SHOW_MESSAGE "DISTANCE HIT!";
}


void Marshmallow_Ticker()
{	
	DroppedItemCleanup();  
	CorpseCleanup();

	DoAllBot_AI();

	PKE_Scanner();

	AutoUse();
	DoTimeouts();
	PushBarrel();
	ResetBarrel();
	DynamicMusic();

	CheckSandboxStatus();
	CheckDeathmatchStatus();
	
	Path_PlayerNodes();  

	AddCmdLineBots();  
	InitMusic();
}


