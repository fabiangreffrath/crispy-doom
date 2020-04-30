#include "marshmallow.h"


static boolean IsGibbableProp(mobj_t* thing)  
{
	int prop = thing->info->spawnstate;

	switch (prop)
	{
		case S_PLAY_DIE7:   // dead player prop
		case S_PLAY_XDIE9:   // XDeath player prop
			return true;
//#if 0
		case S_HANGNOGUTS:   // corpse decorations...
			if (debugmode) { SHOW_MESSAGE "S_HANGNOGUTS"; } return true; 
		case S_HANGTLOOKUP: // map32
			if (debugmode) { SHOW_MESSAGE "S_HANGTLOOKUP"; } return true; 
		case S_HANGBNOBRAIN: // map32
			if (debugmode) { SHOW_MESSAGE "S_HANGBNOBRAIN"; } return true; 
		case S_HANGTLOOKDN:
			if (debugmode) { SHOW_MESSAGE "S_HANGTLOOKDN"; } return true; 
		case S_HANGTSKULL:
			if (debugmode) { SHOW_MESSAGE "S_HANGTSKULL"; } return true; 
		case S_HANGTNOBRAIN: 
			if (debugmode) { SHOW_MESSAGE "S_HANGTNOBRAIN"; } return true; 
		case S_BLOODYTWITCH:
			if (debugmode) { SHOW_MESSAGE "S_BLOODYTWITCH"; } return true; 
		case S_DEADTORSO:
			if (debugmode) { SHOW_MESSAGE "S_DEADTORSO"; } return true; 
		case S_DEADBOTTOM:
			if (debugmode) { SHOW_MESSAGE "S_DEADBOTTOM"; } return true; 
		case S_HEADSONSTICK:
			if (debugmode) { SHOW_MESSAGE "S_HEADSONSTICK"; } return true; 
		case S_HEADONASTICK:
			if (debugmode) { SHOW_MESSAGE "S_HEADONASTICK"; } return true; 
		case S_HEADCANDLES:
			if (debugmode) { SHOW_MESSAGE "S_HEADCANDLES"; } return true; 
		case S_DEADSTICK:
			if (debugmode) { SHOW_MESSAGE "S_DEADSTICK"; } return true; 
		case S_LIVESTICK:
			if (debugmode) { SHOW_MESSAGE "S_LIVESTICK"; } return true; 
		case S_MEAT2:
			if (debugmode) { SHOW_MESSAGE "S_MEAT2"; } return true; 
		case S_MEAT3:
			if (debugmode) { SHOW_MESSAGE "S_MEAT3"; } return true; 
		case S_MEAT4:
			if (debugmode) { SHOW_MESSAGE "S_MEAT4"; } return true; 
		case S_MEAT5:
			if (debugmode) { SHOW_MESSAGE "S_MEAT5"; } return true; 
//#endif
		default:

			return false;
	}
}


static boolean IsThingException(mobj_t* thing)
{
	if (PLAYING_DOOM2)
	{
		if (MAP32 && !Marshmallow_Sandbox
			&& thing->type == MT_KEEN)   // so we don't mess with the keens that trigger exit switch
		{
			return true;
		}

		if (thing->type == MT_SKULL)  // no blood splat for skulls; TODO: smoke splat instead of blood
		{
			return true;
		}

		if (MAP07)
		{
			if (thing->type == MT_BABY    // gibbing these corpses messes up the wall trigger in MAP07
				|| thing->type == MT_FATSO)   // gibbing these corpses messes up the wall trigger in MAP07
			{
				return true;	
			}
		}
	}

	if (PLAYING_DOOM1)
	{
		if (E1M8 && thing->type == MT_BRUISER)  // gibbing these corpses messes up the wall trigger in E1M8
			return true;
	}

	switch (thing->type)
	{
		case MT_BARREL:  // so barrels don't bleed...

		case MT_SPIDER:  // special boss gibs are handled elsewhere
		case MT_CYBORG: 
			return true;
	}


	return false;
}


#define NUM_SLOP_DOOM1 1
#define NUM_SLOP_DOOM2 4

int slop_props[NUM_SLOP_DOOM2] = {

	MT_MISC71,     // large pool
	MT_MISC86,     // spleen   (Doom2 only)
	MT_MISC85,	   // small pool   (Doom2 only)
	MT_MISC84,	   // red and brown pool   (Doom2 only)
};


void DropSlopProp(mobj_t* thing)
{
	int prop;
	int num;

	if (GetGameType() == DOOM1)
		num = NUM_SLOP_DOOM1;   // if playing Doom 1 we only have MISC71 available
	else
		num = NUM_SLOP_DOOM2;   // if playing Doom 2 we have all four available

	prop = GetRandomIntegerInRange(0, num-1); 
		
	P_SpawnMobj(thing->x, thing->y, thing->z, slop_props[prop]); 
}


boolean IsGibbableThing(mobj_t* thing) 
{
	if (!Marshmallow_GibMode/* || realnetgame*/)
		return false;

	if ( IsThingException(thing) )
		return false;

	if ( !IsGibbableProp(thing) && !(thing->flags & MF_CORPSE) )
		return false;
	
	if ( !(thing->corpsehealth) || Marshmallow_GibMode == ROTT_GIBS )
		return false;

	if (!IsGibbableProp(thing) && thing->tics > 0)  // so they don't gib immediately on death
		return false;

	thing->corpsehealth = NULL;

	CorpseGib(thing);
	
	if ( IsPlayer(thing) )  // don't do the below stuff if thing is player
		return false;

	DropSlopProp(thing);
	
	P_RemoveMobj(thing);  

	return true;
}


#define BRUTAL_GIBCHANCE 60

void GibFactory(mobj_t* target, mobj_t* source)
{
	int negative_spawnhealth = target->info->spawnhealth * -1; 
	int splat_chance = negative_spawnhealth / 5; // was /4
	int gib_mode = Marshmallow_GibMode;

	if ( target->type == MT_SKULL
		|| IsBarrel(target) )
		return;

	switch (gib_mode)  
	{
	case BRUTAL_GIBS:	

		if (/*target->health < splat_chance
			||*/ P_CheckMeleeRange(target)
			&& IsPlayer(source)  //  was IsConsolePlayer()
			&& PercentChance( BRUTAL_GIBCHANCE ) ) // don't do this every time
		{
			BrutalSplat(target); 
			
			if (target->info->xdeathstate)
			P_SetMobjState (target, target->info->xdeathstate);
		}
		
		break;

	case ROTT_GIBS:
	case DUKE_GIBS:

		if (target->health < splat_chance
			&& !IsBarrel(target)
			&& !IsPlayer(target) )
		{
			if ( IsPlayer(source) )
			{
				if (source->player->readyweapon != wp_missile)
					break;
			}

			ParticleFX_XDeath(target);
			
			//if (Marshmallow_GibMode != DUKE_GIBS)
				DropSlopProp(target);

			P_RemoveMobj(target);
			return;
		}

		break;
	}
}