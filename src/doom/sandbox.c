//////////////////////////////////
//
//  SANDBOX.C:  Sandbox game mode
//
//////////////////////////////////

#include "marshmallow.h"
#include "pkemeter.h"


// TODO: rename and move to tweaks.c; maybe keep this function as a wrapper for our new random item function
int PlaceSandboxItem(int i)
{  
	switch (i)
	{
	case MT_CLIP:
	case MT_SHELLS:
	case MT_ROCKET:
	case MT_CELL:
				
		i = GetRandomBonusItem(AMMO);
		return i;

	case MT_SHOTGUN:
	case MT_SUPERSHOTGUN:
	case MT_CHAINGUN:
	case MT_RPG:
	case MT_PLASMA_RIFLE:
	case MT_BFG9000:

		i = GetRandomBonusItem(WEAPON);
		return i;

	case MT_HEALTH_BONUS:    

		i = GetRandomBonusItem(AMMO);
		
		return i;

	case MT_ARMOR_BONUS:

		i = GetRandomBonusItem(WEAPON);
		return i;

	default:
		return i;
	}
}


static void SandboxVictory()
{
	sandbox.design_mode = true;
	sandboxhelp_on = true;

	PlayBonusSound(LONGBLIP);
	SHOW_CENTERED_MESSAGE DEH_String(SANDBOXDONE);

	// for stats
	level_stats.sandbox_games_won = 1;
	SaveStats();
	memset(&level_stats, NULL, sizeof(level_stats));

	if (Marshmallow_DynamicMusic)
		DJ_StartPlaylist(VICTORY);
}


void CheckSandboxStatus()  
{
	if (!Marshmallow_Sandbox)
		return;
	
	if (sandbox.design_mode)   
	{
		int i = 0;

		if (!MAIN_PLAYER.cards[i]) // if we don't have any keys, give all keys for sandbox mode
		{
			for (i=0 ; i<NUMCARDS ; i++)
				MAIN_PLAYER.cards[i] = true;
		}
	}
	
	else if ( !PKE_MonstersInArea() )   
	{
		SandboxVictory();
	}
}


void ChangeThingType(int player, direction_t direction)
{
	player_t* p;
	p = &players[player];

	switch (direction)
	{
	case BACKWARD:

		if (p->sandbox_object > 1)
		{
			p->sandbox_object--;

			if (p->sandbox_object == MT_BARREL - 1)	// used to go only go to MT_KEEN
				p->sandbox_object -= 5;   // was 1 for MT_KEEN
			else if (p->sandbox_object == MT_BRUISERSHOT)	
				p->sandbox_object -= 1;   
			else if (p->sandbox_object == MT_FATSHOT)	
				p->sandbox_object -= 1;   
			else if (p->sandbox_object == MT_SMOKE)	
				p->sandbox_object -= 2;   
			else if (p->sandbox_object == MT_FIRE)	
				p->sandbox_object -= 1;   
		}
		
		break;

	case FORWARD:

		if (p->sandbox_object < MT_BARREL)	// used to only go to MT_KEEN
		{
			p->sandbox_object++;

			if (p->sandbox_object == MT_BOSSBRAIN)	
				p->sandbox_object += 5;      // was 1 for MT_KEEN
			else if (p->sandbox_object == MT_BRUISERSHOT)	
				p->sandbox_object += 1;   
			else if (p->sandbox_object == MT_FATSHOT)	
				p->sandbox_object += 1;   
			else if (p->sandbox_object == MT_TRACER)	
				p->sandbox_object += 2;   
			else if (p->sandbox_object == MT_FIRE)	
				p->sandbox_object += 1;   
		}

		break;
	}
}


static boolean CheckThingType(mobjtype_t thing)
{
	if (!thing)
		return false;

	if (gamemode != commercial
		&& !Marshmallow_WadStealing)
	{
		if ( thing == MT_VILE
		||	 thing == MT_UNDEAD
		||   thing == MT_KEEN
		||   thing == MT_WOLFSS
		||   thing == MT_PAIN
		||   thing == MT_BABY
		||   thing == MT_KNIGHT
		||   thing == MT_CHAINGUY
		||   thing == MT_FATSO )
		{
			SHOW_MESSAGE DEH_String(NOWADSTEALING);
			return false;
		}
		else
			return true;
	}

	return true;  // added 1-21-20 because this was returning zero in release builds
}


void CancelSandbox()
{
	memset (&sandbox, 0, sizeof(sandbox));  // Clear out saved sandbox info

	Marshmallow_Sandbox = false;
	sandbox.design_mode = false;
	sandboxhelp_on = false;
	Marshmallow_CoopItemRespawn = false;

	Doom1SSG_Level = DEFAULT_SSG_LEVEL;
}


void InitSandbox()
{
	if (!netgame && gamemap == saved_sandbox.map)  // added map check on 3-9-19
		RestoreSavedSandbox();   // in single player sandbox, this is called every time player dies and map is reloaded
	else
		memset (&sandbox, 0, sizeof(sandbox)); 

	Doom1SSG_Level = 1;

	Marshmallow_Sandbox = true;
	sandbox.design_mode = true;
	sandboxhelp_on = true;
	Marshmallow_CoopItemRespawn = true;
}


void ResetSandbox()
{
	int i;

	if (sandbox.design_mode == false)
	{
		SHOW_MESSAGE "CANNOT RESET DURING BATTLE!";  
		return;
	}

	for (i = 1; i <= sandbox.count; i++)   // starting at 1 because we don't use slot 0 in sandbox.monster array
		P_RemoveMobj(sandbox.monster[i]);

	memset (&sandbox, 0, sizeof(sandbox)); 
	sandbox.design_mode = true;
}


void UnleashTheHordes()
{
	int i;
	int max = sandbox.count;

	if ( !PKE_MonstersInArea() )   
	{
		SHOW_MESSAGE DEH_String(NOMONSTERS);
		
		return;
	}

	sandbox.design_mode = false;
	sandboxhelp_on = false;

	SHOW_MESSAGE DEH_String(FIGHT);

	S_StartSound(NULL, sfx_sgcock); 

	saved_sandbox.count = sandbox.count;  // NEW

	for (i = 0; i <= max; i++)  
	{
		if (!sandbox.monster[i])   // need this because currently we leave the zero index empty
			continue;

		//saved_sandbox.monster[i] = sandbox.monster[i];
		saved_sandbox.monster_type[i] = sandbox.monster_type[i];
		saved_sandbox.monster_x[i] = sandbox.monster_x[i];
		saved_sandbox.monster_y[i] = sandbox.monster_y[i];

		saved_sandbox.monster_angle[i] = sandbox.monster_angle[i];  // testing

		saved_sandbox.map = gamemap;  // added 3-9-19
		
		if (!realnetgame)  // only because for some reason we can't unset this flag in realnetgame
		sandbox.monster[i]->flags ^= MF_TRANSLUCENT;   // WTF: why doesn't this operation work in realnetgame?
		
		sandbox.monster[i]->flags |= MF_SHOOTABLE;	

		if (sandbox.monster[i]->type != MT_KEEN)  // NOTE: keens are no longer in this list
		sandbox.monster[i]->flags |= MF_SOLID;	  
	}

	// NOTE: ditching any sound effect for now
	//S_StartSound(NULL, sfx_metal);   

	if (Marshmallow_DynamicMusic)
		DJ_StartPlaylist(INTENSE);
}


void RestoreSavedSandbox()  // seems that we keep coming in here twice in a row  (called from both SkipToLevel() and InitSandbox()
{
	if (!Marshmallow_Sandbox)
		return;

	if (saved_sandbox.count > 0)
	{
		int i, max, x, y, type;
		max = sandbox.count = saved_sandbox.count;
		
		for (i=0;i<=max;i++)
		{
			x = saved_sandbox.monster_x[i];
			y = saved_sandbox.monster_y[i];
			type = saved_sandbox.monster_type[i];

			sandbox.monster[i] = P_SpawnMobj (x, y, ONFLOORZ, type);  // ONFLOORZ?

			sandbox.monster[i]->angle = saved_sandbox.monster_angle[i];  // NEW testing
			
			sandbox.monster[i]->flags &= MF_SOLID;	  
			sandbox.monster[i]->flags &= MF_SHOOTABLE;	
			sandbox.monster[i]->flags |= MF_TRANSLUCENT;
		}
		
		SHOW_MESSAGE DEH_String(RESTORESANDBOX);
	}
}


void PlaceMonster(mobj_t* actor)  
{
	angle_t	an;
	fixed_t		x, y, z;
	mobjtype_t	item_type;
	
	if (actor->player->playerstate == PST_DEAD)  
	{
		actor->player->message = "CANNOT PLACE MONSTERS WHILE DEAD!";
		return;
	}

	if (actor->player->DropObjectDelay)  
	{
		actor->player->message = "WAIT A MOMENT TO PLACE ANOTHER.";
		return;
	}
	else
	{
		actor->player->DropObjectDelay = OBJECT_DROP_DELAY;
	}

	item_type =  actor->player->sandbox_object;		

	if ( !CheckThingType(item_type) )  
	{
		SHOW_MESSAGE "CHECKTHINGTYPE FAILED!";
		return;
	}

	an = actor->angle >> ANGLETOFINESHIFT;
	
	x = actor->x ; 
	y = actor->y ; 

	if (item_type == MT_KEEN)    
	{
		mobj_t* mobj; 
		z = ONCEILINGZ;
		mobj = P_SpawnMobj (x, y, z, item_type);
		mobj->flags ^= MF_SOLID; 

		return;   // don't count keens in our sandbox enemy list
	}
	
	// Monsters only:
	
	z = ONFLOORZ;

	sandbox.count++;

	sandbox.monster[ sandbox.count ] = P_SpawnMobj (x, y, ONFLOORZ, item_type);
	sandbox.monster_x [ sandbox.count ] = x;
	sandbox.monster_y [ sandbox.count ] = y;
	sandbox.monster_type [ sandbox.count ] = item_type;
	//sandbox.monster[ sandbox.count ]->angle = actor->angle+ANG180;
	sandbox.monster[ sandbox.count ]->angle = actor->angle+ANG180;  // testing
	sandbox.monster_angle[ sandbox.count ] = actor->angle+ANG180;  // testing
	
	sandbox.monster[sandbox.count]->flags &= MF_SOLID;	  
	sandbox.monster[sandbox.count]->flags &= MF_SHOOTABLE;	

	if (!realnetgame)
	sandbox.monster[sandbox.count]->flags |= MF_TRANSLUCENT;

	if (item_type == MT_BARREL)
		sandbox.monster[sandbox.count]->flags |= MF_SPECIAL;  // so we can push barrels

	actor->player->message = "SPAWNING OBJECT.";
}


