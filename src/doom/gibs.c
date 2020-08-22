#include "marshmallow.h"


static boolean IsGibbableProp(mobj_t* thing)  
{
	int prop = thing->info->spawnstate;

	switch (prop)
	{
		case S_PLAY_DIE7:   // Dead player prop
		case S_PLAY_XDIE9:  // XDeath player prop
			return true;

		// Corpse decorations
		case S_HANGNOGUTS:
			if (debugmode) { SHOW_MESSAGE "S_HANGNOGUTS"; } return true; 
		case S_HANGTLOOKUP:
			if (debugmode) { SHOW_MESSAGE "S_HANGTLOOKUP"; } return true; 
		case S_HANGBNOBRAIN:
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

		default:
			return false;
	}
}


static boolean IsThingException(mobj_t* thing)
{
	if (PLAYING_DOOM2)
	{
		if (MAP32 && !Marshmallow_Sandbox
			&& thing->type == MT_KEEN)   // So we don't mess with the keens that trigger exit switch
		{
			return true;
		}

		if (thing->type == MT_SKULL)  // No blood splat for skulls
		{
			return true;
		}

		if (MAP07)
		{
			if (thing->type == MT_BABY    // Gibbing these corpses messes up the wall trigger in MAP07
				|| thing->type == MT_FATSO)   // Gibbing these corpses messes up the wall trigger in MAP07
			{
				return true;	
			}
		}
	}

	if (PLAYING_DOOM1)
	{
		if (E1M8 && thing->type == MT_BRUISER)  // Gibbing these corpses messes up the wall trigger in E1M8
			return true;
	}

	return false;
}


#define NUM_SLOP_DOOM1 1
#define NUM_SLOP_DOOM2 4

int slop_props[NUM_SLOP_DOOM2] = {

	MT_MISC71,     // large pool
	MT_MISC86,     // Spleen   (Doom2 only)
	MT_MISC85,	   // Small pool   (Doom2 only)
	MT_MISC84,	   // Red and brown pool   (Doom2 only)
};


static void DropSlopProp(mobj_t* thing, boolean corpse)
{
	int prop;
	int num;
	fixed_t z = thing->z;

	if (!corpse)
	    z += DROP_FROM_ABOVE_FLOOR;

	if (GetGameType() == DOOM1)
		num = NUM_SLOP_DOOM1;   // If playing Doom 1 we only have MISC71 available
	else
		num = NUM_SLOP_DOOM2;   // If playing Doom 2 we have all four available

	prop = GetRandomIntegerInRange(0, num-1); 
		
	P_SpawnMobj(thing->x, thing->y, z, slop_props[prop]);
}


#define SLOP_CHANCE 40
#define SLOP_TIMEOUT 50

void PlaySlopSound(mobj_t* target)
{
    static int timeout;
    int chance = PercentChance(SLOP_CHANCE);

    // Check our timeout so we don't play this sound too often
    if (!timeout)
    {
        timeout = SLOP_TIMEOUT;
    }
    else
    {
        timeout--;
        return;
    }

    if (chance)
        S_StartSound(target, sfx_slop);
}


static void RandomSplat(mobj_t* target)
{
    int chance = PercentChance(GIB_CHANCE);

    if (chance)
    {
        ParticleFX_TinyBloodSplat(target);
    }
    else
    {
        ParticleFX_SmallBloodSplat(target);
    }
}


static void BrutalDeath(mobj_t* target)
{
    RandomSplat(target);
    PlaySlopSound(target);

    if (target->info->xdeathstate)
        P_SetMobjState (target, target->info->xdeathstate);
}


static void ApogeeDeath(mobj_t* target)
{
    ParticleFX_LargeBloodSplat(target);

    DropSlopProp(target, false);

    P_RemoveMobj(target);
}

#define DEATH_CHANCE_RATIO 50

static void RandomDeath(mobj_t* target)
{
    int chance = PercentChance(DEATH_CHANCE_RATIO);

    if (chance)
    {
        BrutalDeath(target);
    }
    else
    {
        ApogeeDeath(target);
    }
}


boolean IsGibbableThing(mobj_t* thing)
{
	if (!Marshmallow_GibMode)
		return false;

	if ( IsThingException(thing) )
		return false;

	if ( !IsGibbableProp(thing) && !(thing->flags & MF_CORPSE) )
		return false;
	
	if ( !(thing->corpsehealth) || Marshmallow_GibMode == ROTT_GIBS )
		return false;

    // So they don't gib immediately on death
	if (!IsGibbableProp(thing) && thing->tics > 0)
		return false;

	thing->corpsehealth = NULL;

	CorpseGib(thing);

    // Don't do the below stuff if thing is player
	if ( IsPlayer(thing) )
		return false;

	DropSlopProp(thing, true);

    PlaySlopSound(thing);
	
	P_RemoveMobj(thing);  

	return true;
}


static boolean SpecificMonsterDeaths(mobj_t* thing)
{
    switch (thing->type)
    {
        case MT_CYBORG:
        case MT_SPIDER:
            DoBossExplosions(thing);
            return true;

        case MT_BABY:
            ParticleFX_BabyDeath(thing);
            return true;

        // TODO: barrel effects

        default:
            return false;
    }
}


boolean DoSpecialDeaths(mobj_t* target, mobj_t* source)
{
    int negative_spawnhealth = target->info->spawnhealth * -1;
    gibmode_t gibmode = Marshmallow_GibMode;
    weapontype_t weapon;

    // Certain gore events are weapon-dependent
    if (source && source->player)
        weapon = source->player->readyweapon;

    // If we're using pistol, don't do any gib stuff
    if (IsPlayer(target) && weapon == wp_pistol)
        return false;

    if ( SpecificMonsterDeaths(target) )
        return false;

    // Check if the target has an xdeathstate animation
    if (!target->info->xdeathstate)
        return false;

    // Determine if we're doing any special deaths
    switch (gibmode)
    {
        case NO_GIBS:

            return false;

        case BRUTAL_GIBS:

            if (target->health <= negative_spawnhealth/2
                || P_CheckMeleeRange(target))
            {
                if (weapon < wp_missile && weapon != wp_fist)
                    return false;

                if ( IsPlayer(target) )
                    return false;

                BrutalDeath(target);

                return true;
            }

            break;

        case ROTT_GIBS:
        case DUKE_GIBS:

            if (weapon != wp_missile && weapon != wp_fist)
                return false;

            if ( IsPlayer(target) )
                return false;

            ApogeeDeath(target);

            return true;
    }

    return false;
}


void DoChainsawBlood(mobj_t* target, mobj_t* inflictor)
{
    if ( IsMonster(target)
         && IsPlayer(inflictor)
         && Marshmallow_GibMode == BRUTAL_GIBS
         && inflictor->player->readyweapon == wp_chainsaw
         && target->type != MT_SKULL )
    {
        ParticleFX_ChainsawBloodSplat(target);
        PlaySlopSound(target);
    }
}


void DoCriticalHitBlood(mobj_t* target)
{
    int sound = target->info->painsound;
    int health = target->health;
    int lowhealth_threshold = CRITICALHIT_THRESHOLD;

    // Barrels dont bleed
    if ( IsBarrel(target) )
        return;

    // Don't do this for players
    if ( IsPlayer(target) )
        return;

    // Don't do this if target is dead
    if (!health)
        return;

    // Don't do this if health isn't low enough
    if (health > lowhealth_threshold)
        return;

    // Only do this in Brutal gore mode
    if (Marshmallow_GibMode != BRUTAL_GIBS)
        return;

    // Check if we've already done this
    if (target->critical_injury)
    {
        switch (target->type)
        {
            // Exceptions for boss monsters
            case MT_SPIDER:
            case MT_CYBORG:
            case MT_BRUISER:
                //break;

            default:
                return;
        }
    }

    // Set our "critical injury" flag so that we know
    // to only come in here once for most enemies
    target->critical_injury = true;

    // Don't do this every time
    if (PercentChance(GIB_CHANCE))
        return;

    // Don't do this for explosive weapons
    if (target->player && target->player->readyweapon == wp_missile)
        return;

    ParticleFX_TinyBloodSplat(target);
}