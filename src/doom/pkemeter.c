/////////////////////////////////////////////////
//
//  PKEMETER.C: Danger tracking metrics
//
/////////////////////////////////////////////////

#include "pkemeter.h"

// Returns number of living monsters in map, or false if none found
int PKE_MonstersInArea()   
{
    thinker_t*	th;
    mobj_t*	mo;
	int monsters = 0;

    for (th = thinkercap.next ; th != &thinkercap ; th=th->next)
    {
		if (th->function.acp1 != (actionf_p1)P_MobjThinker)
			continue;
	
		mo = (mobj_t *)th;
		if ( IsMonster(mo) )
		{
			if ( mo->health > 0 )
			{
				monsters++;
			}
		}
    }

	return monsters;
}


int PKE_GetDangerLevel()
{
	int dangerlevel = PKE_Meter.attackers_hp;

	return dangerlevel;
}


static boolean MonsterIsBoss(mobj_t* monster)  
{
	pkemeter_t*  pke = &PKE_Meter;

	if ( pke->bossfight || Marshmallow_Sandbox )
		return false;

	if ( !P_CheckSight(monster, MAIN_PLAYER.mo) )
		return false;

	switch ( monster->type )
	{
	case MT_BRUISER:

		if (PLAYING_DOOM2)
			return false;   

		if ( !E1M8 ) 
			return false;

		if ( deathmatch )
			return false;

		if (pke->bossfight)  // 3-6-20 ... so we don't trigger boss music twice in E1M8
			return false;

		pke->bossfight = true;
		pke->boss = monster; 

		if (Marshmallow_DynamicMusic)  
			DJ_StartPlaylist(BOSS);   

		return true;

	case MT_CYBORG:

		if (PLAYING_DOOM2)  
		{
			if (!MAP32)    // In Doom 2 only the Cyberdemon in MAP32 will be considered a boss fight
				return false;
		}
						
		if (PLAYING_DOOM1)
		{
			if (!E2M8)	// In Doom only the Cyberdemon in E2M8 will be considered a boss fight
				return false;
		}

		if ( deathmatch )
			return false;

		pke->bossfight = true;
		pke->boss = monster;

		if (Marshmallow_DynamicMusic)
			DJ_StartPlaylist(BOSS);

		return true;

	case MT_SPIDER:

		if (PLAYING_DOOM2)
			return false;   

		if ( !E3M8 ) 
			return false;

		if ( deathmatch )
			return false;

		pke->bossfight = true;
		pke->boss = monster;

		if (Marshmallow_DynamicMusic)
			DJ_StartPlaylist(BOSS);

		return true;

	default:

		return false;
	}
}


// Builds our attackers list
static void PKE_CalculateDanger()  
{
	int i;
	int total = PKE_Meter.monsters_detected; 
	int attackers = 0;
	int attackers_hp = 0;

	for (i=0; i<total; i++)
	{
		mobj_t* monster;

		if ( !PKE_Meter.monsters[i] )  
			continue;

		monster = PKE_Meter.monsters[i];

		if (monster->attacking_player != 1) 
			continue;
		
		if ( P_CheckSight( monster, MAIN_PLAYER.mo ) )  
		{
            // Don't count attackers far away
            if ( !EnemyInRange(monster) )
                continue;

			attackers++;
			attackers_hp += THIS_MOBJ_HP;  
		}
	}

	if (!attackers)
		PKE_Meter.safety_time++;
	else
		PKE_Meter.safety_time = 0;

	PKE_Meter.attackers_total = attackers; 
	PKE_Meter.attackers_hp = attackers_hp;
}


static void RemoveMonsterFromRadar(mobj_t* monster)
{
	monster->PKE__on_radar = false;
	PKE_Meter.monsters_onradar--;
	PKE_Meter.healthpoints_onradar -= THIS_MOBJ_HP;
}


static void AddMonsterToRadar(mobj_t* monster)
{
    monster->PKE__on_radar = true;
	PKE_Meter.monsters_onradar++;
	PKE_Meter.healthpoints_onradar += THIS_MOBJ_HP;
}


static void AddMonsterToList(mobj_t* monster)  
{
	int new_index;
		
	PKE_Meter.monsters_detected++;
	PKE_Meter.healthpoints_detected += THIS_MOBJ_HP;

	new_index = PKE_Meter.monsters_detected;
	PKE_Meter.monsters[ new_index ] = monster;

	monster->already_indexed = true;
}


// Check if detected monster is within our radar range
static boolean PKE_CheckDistance(mobj_t* thing) 
{
	int distance;
	int radius = PKE_Meter.search_radius;;
	mobj_t* player = MAIN_PLAYER.mo;

	distance = P_AproxDistance (player->x - thing->x, 
 					player->y - thing->y);

	if (distance > radius)
	{
		return false; 
	}
	else
	{
		return true;
	}
}


// Builds an all-knowing list of every monster in the area
static void ScanArea()
{
	thinker_t*	th;
    mobj_t*	    mo;

	for (th = thinkercap.next ; th != &thinkercap ; th=th->next)
    {
		if (th->function.acp1 != (actionf_p1)P_MobjThinker)
			continue;
	
		mo = (mobj_t *)th;

		if ( IsMonster(mo) )
		{
			if ( mo->health <= 0 )  
				continue;

			if ( MonsterIsBoss(mo) )  
				return;

			if ( mo->target == MAIN_PLAYER.mo 
				&& !mo->attacking_player )
			{
				mo->attacking_player = true;  
			}

			if ( mo->PKE__on_radar )   
			{
				if ( mo->health <= 0 )  
					RemoveMonsterFromRadar(mo); 
				else if ( !PKE_CheckDistance(mo) )
					RemoveMonsterFromRadar(mo);
				else
					continue;
			}

			if ( !mo->already_indexed )
				AddMonsterToList(mo);

			if ( PKE_CheckDistance(mo) )
				AddMonsterToRadar(mo);
		}
    }
}


void PKE_KillBoss()
{
	if (!PKE_Meter.bossfight)  
		return;  // Only come in here once

	PKE_Meter.bossfight = false; 
	
	level_stats.bosses_killed = 1;
}


static boolean PKE_CheckBoss()     
{
	if (!modifiedgame && E1M8)  // Prevent E1M8 bosses triggering bossfight twice
		return true;

	if (PKE_Meter.boss)
	{
		if (PKE_Meter.boss->health <= 0)
		{
			PKE_KillBoss();
			return false;
		}

		return true;  // So it doesn't restart boss fight
	}
	else
	{
		return false;
	}
}


void PKE_KillEntity(mobj_t* entity)  
{
	int monsters_left;

	if (PKE_Meter.monsters_remaining)
		PKE_Meter.monsters_remaining--;
	
	entity->attacking_player = false;  
	
	if (entity->PKE__on_radar)
	{
		entity->PKE__on_radar = false;
		PKE_Meter.healthpoints_onradar -= (mobjinfo[ entity->type ].spawnhealth * MonsterHitpointsScale);
		PKE_Meter.monsters_onradar--;
	}

	// Update monsters_remaining every time we kill a monster
	monsters_left = PKE_Meter.monsters_remaining = PKE_MonstersInArea();

	// Check for all kills
	if ( !monsters_left )
		AllKills_Achievement();
}


void BuildObjList()  
{
	PKE_Meter.monsters_remaining++; 
	PKE_Meter.monsters_in_level++; 
}


static boolean PKE_DrawDelay()
{
	if (PKE_Meter.draw_delay)
	{
		PKE_Meter.draw_delay--;
		return true;  // Abort calling function
	}
	else
	{
		PKE_Meter.draw_delay = DRAW_DELAY;
		return false; // Continue through calling function
	}
}


void PKE_HUDisplay()
{
	int dangerlevel = PKE_Meter.healthpoints_onradar;

	if ( PKE_DrawDelay() )
		return;

	if (dangerlevel <= DANGERLEVEL_LOW)      
	{
		HUlib_addMessageToSText(&hellscape_text, 0, DEH_String(DANGERSYMBOL1));
		return;
	}

	if (dangerlevel <= DANGERLEVEL_MEDIUM)  
	{
		HUlib_addMessageToSText(&hellscape_text, 0, DEH_String(DANGERSYMBOL2));
		return;
	}

	if (dangerlevel <= DANGERLEVEL_HIGH)  
	{
		HUlib_addMessageToSText(&hellscape_text, 0, DEH_String(DANGERSYMBOL3));
		return;
	}

	if (dangerlevel <= DANGERLEVEL_CRITICAL)  
	{
		HUlib_addMessageToSText(&hellscape_text, 0, DEH_String(DANGERSYMBOL4));
		return;
	}
	
	if (dangerlevel <= DANGERLEVEL_FUBAR)  
	{
		HUlib_addMessageToSText(&hellscape_text, 0, DEH_String(DANGERSYMBOL5));
		return;
	}
}


void PKE_Reset()
{
	memset (&PKE_Meter, 0, sizeof(PKE_Meter)); 

	PKE_Activate();
}


void PKE_Activate()
{
    // Default to medium since we haven't yet read our saved setting from cfg file
	PKE_Meter.search_radius = PKE_RADIUS_MEDIUM;
}


void AllKills_Achievement() 
{
	if (gamemap == 30)  // Don't do this on MAP30 since monsters are respawned indefinitely
		return;

	if (gameskill == sk_nightmare && Marshmallow_RespawnInNightmare) 
		return;    

	if (Marshmallow_Sandbox || deathmatch)
		return;

	PlayBonusSound(LONGBLIP);
		
	MAIN_PLAYER.bonuscount += BONUSADD;
	SHOW_CENTERED_MESSAGE DEH_String(ALLMONSTERS);

	level_stats.allkills++;
}


void PKE_ShowInfo()  
{
	pkemeter_t*  pke = &PKE_Meter;

	if (!pke)
		return;

	// TODO: color

	AddToInfoReadout("on_radar: ", pke->monsters_onradar, 2);
	AddToInfoReadout("healthpoints_onradar: ", pke->healthpoints_onradar,3);

	AddToInfoReadout("attackers_total: ", pke->attackers_total, 5);
	AddToInfoReadout("attackers_hp: ", pke->attackers_hp, 6);

	AddToInfoReadout("highdanger_threshold: ", pke->highdanger_threshold, 8);

	//AddToInfoReadout("monsters_in_level: ", pke->monsters_in_level, 7);
	//AddToInfoReadout("monsters_remaining: ", pke->monsters_remaining, 8);
}


void PKE_Readout()
{
	pkemeter_t*  pke = &PKE_Meter;

	int onradar = pke->monsters_onradar;
	int hp_onradar = pke->healthpoints_onradar;
	int attackers = pke->attackers_total;
	int hp_attackers = pke->attackers_hp;

	int color;

	char string[16];

	if (onradar > 0)
	{
		color = POSITIVE_COLOR;
		sprintf(string, "%d", onradar);
	}
	else
	{
		color = NULL_COLOR;
		sprintf(string, "%s", ZERO_STRING);
	}

	CrispyReplaceColor(string, color, string);
	HUlib_addMessageToSText(&pkeline1, "On Radar: ", DEH_String(string) );
	
	if (hp_onradar > 0)
	{
		color = POSITIVE_COLOR;
		sprintf(string, "%d", hp_onradar);
	}
	else
	{
		color = NULL_COLOR;
		sprintf(string, "%s", ZERO_STRING);
	}

	CrispyReplaceColor(string, color, string);
	HUlib_addMessageToSText(&pkeline2, "Area Risk: ", DEH_String(string) );

	if (attackers > 0)
	{
		color = POSITIVE_COLOR;
		sprintf(string, "%d", attackers);
	}
	else
	{
		color = NULL_COLOR;
		sprintf(string, "%s", ZERO_STRING);
	}
	
	CrispyReplaceColor(string, color, string);
	HUlib_addMessageToSText(&pkeline3, "Attackers: ", DEH_String(string) );

	if (hp_attackers > 0)
	{
		color = POSITIVE_COLOR;
		sprintf(string, "%d", hp_attackers);
	}
	else
	{
		color = NULL_COLOR;
		sprintf(string, "%s", ZERO_STRING);
	}
	
	CrispyReplaceColor(string, color, string);
	HUlib_addMessageToSText(&pkeline4, "Danger: ", DEH_String(string) );

    // Show range/radius selection at the bottom
    HUlib_addMessageToSText(&pkeline8, DEH_String(PKEHINT_RADIUS), DisplayPKERadius() );
    CrispyReplaceColor(PKEHINT_RADIUS, CR_GRAY, PKEHINT_RADIUS);
}


static boolean PKE_Heartbeat()
{
	// Regulates the pulse rate of our PKE search waves
	if (PKE_Meter.heartbeat)
	{
		PKE_Meter.heartbeat--;
		return false;	// Still waiting
	}
	else
	{
		PKE_Meter.heartbeat = PKE_RECALC_DELAY;  
		return true;	 // Continue through calling function
	}
}


void PKE_Scanner()  
{
	if ( !PKE_Heartbeat() )
		return;

	ScanArea();  // Builds overall monster list and handles boss encounters

	PKE_CalculateDanger();  // Detects attackers and updates their status based on P_CheckSight()

	PKE_CheckBoss();  // Checks status of boss monster during bossfights
}