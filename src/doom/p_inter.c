//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//	Handling interactions (i.e., collisions).
//




// Data.
#include "doomdef.h"
#include "dstrings.h"
#include "sounds.h"

#include "deh_main.h"
#include "deh_misc.h"
#include "doomstat.h"

#include "m_random.h"
#include "i_system.h"

#include "am_map.h"

#include "p_local.h"

#include "s_sound.h"

#include "p_inter.h"

// [marshmallow]
#include "marshmallow.h"
#include "pkemeter.h"
int thrust_multiplier[NUMPHYSICSMODES] = { DEFAULT_THRUST, MEDIUM_THRUST_MULTIPLIER, HIGH_THRUST_MULTIPLIER };

#define BONUSADD	6




// a weapon is found with two clip loads,
// a big item has five clip loads
int	maxammo[NUMAMMO] = {200, 50, 300, 50};
int	clipammo[NUMAMMO] = {10, 4, 20, 1};


//
// GET STUFF
//

//
// P_GiveAmmo
// Num is the number of clip loads,
// not the individual count (0= 1/2 clip).
// Returns false if the ammo can't be picked up at all
//

boolean
P_GiveAmmo
( player_t*	player,
  ammotype_t	ammo,
  int		num,
  boolean	dropped ) // [NS] Dropped ammo/weapons give half as much.
{
    int		oldammo;
	
    if (ammo == am_noammo)
	return false;
		
    if (ammo > NUMAMMO)
	I_Error ("P_GiveAmmo: bad type %i", ammo);
		
    if ( player->ammo[ammo] == player->maxammo[ammo]  )
	return false;
		
    if (num)
	num *= clipammo[ammo];
    else
	num = clipammo[ammo]/2;
    
    if (gameskill == sk_baby
	|| gameskill == sk_nightmare)
    {
	// give double ammo in trainer mode,
	// you'll need in nightmare
	num <<= 1;
    }
    
	// [NS] Halve if needed.
	if (dropped)
	{
		num >>= 1;
		// Don't round down to 0.
		if (!num)
			num = 1;
	}
		
    oldammo = player->ammo[ammo];
    player->ammo[ammo] += num;

    if (player->ammo[ammo] > player->maxammo[ammo])
	player->ammo[ammo] = player->maxammo[ammo];

    // If non zero ammo, 
    // don't change up weapons,
    // player was lower on purpose.
    if (oldammo)
	return true;	

    // We were down to zero,
    // so select a new weapon.
    // Preferences are not user selectable.
    switch (ammo)
    {
      case am_clip:
	if (player->readyweapon == wp_fist)
	{
	    if (player->weaponowned[wp_chaingun])
		player->pendingweapon = wp_chaingun;
	    else
		player->pendingweapon = wp_pistol;
	}
	break;
	
      case am_shell:
	if (player->readyweapon == wp_fist
	    || player->readyweapon == wp_pistol)
	{
	    if (player->weaponowned[wp_shotgun])
		player->pendingweapon = wp_shotgun;
	}
	break;
	
      case am_cell:
	if (player->readyweapon == wp_fist
	    || player->readyweapon == wp_pistol)
	{
	    if (player->weaponowned[wp_plasma])
		player->pendingweapon = wp_plasma;
	}
	break;
	
      case am_misl:
	if (player->readyweapon == wp_fist)
	{
	    if (player->weaponowned[wp_missile])
		player->pendingweapon = wp_missile;
	}
      default:
	break;
    }
	
    return true;
}


// [crispy] show weapon pickup messages in multiplayer games
const char *const WeaponPickupMessages[NUMWEAPONS] =
{
	NULL, // wp_fist
	NULL, // wp_pistol
	GOTSHOTGUN,
	GOTCHAINGUN,
	GOTLAUNCHER,
	GOTPLASMA,
	GOTBFG9000,
	GOTCHAINSAW,
	GOTSHOTGUN2,
};

//
// P_GiveWeapon
// The weapon name may have a MF_DROPPED flag ored in.
//
boolean
P_GiveWeapon
( player_t*	player,
  weapontype_t	weapon,
  boolean	dropped )
{
    boolean	gaveammo;
    boolean	gaveweapon;
	
    if (netgame
	&& (deathmatch!=2)
	 && !dropped )
    {
	// leave placed weapons forever on net games
	if (player->weaponowned[weapon])
	    return false;

	player->bonuscount += BONUSADD;
	player->weaponowned[weapon] = true;

	if (deathmatch)
	    P_GiveAmmo (player, weaponinfo[weapon].ammo, 5, false);
	else
	    P_GiveAmmo (player, weaponinfo[weapon].ammo, 2, false);
	player->pendingweapon = weapon;
	// [crispy] show weapon pickup messages in multiplayer games
	player->message = DEH_String(WeaponPickupMessages[weapon]);

	if (player == &players[consoleplayer])
	    S_StartSound (NULL, sfx_wpnup);

	if (!Marshmallow_WeaponsStay)  // [marshmallow] Leave the weapon on the ground
        return true;

	return false;
    }
	
    if (weaponinfo[weapon].ammo != am_noammo)
    {
	// give one clip with a dropped weapon,
	// two clips with a found weapon
	// [NS] Just need to pass that it's dropped.
	gaveammo = P_GiveAmmo (player, weaponinfo[weapon].ammo, 2, dropped);
	/*
	if (dropped)
	    gaveammo = P_GiveAmmo (player, weaponinfo[weapon].ammo, 1);
	else
	    gaveammo = P_GiveAmmo (player, weaponinfo[weapon].ammo, 2);
	*/
    }
    else
	gaveammo = false;
	
    if (player->weaponowned[weapon])
	gaveweapon = false;
    else
    {
	gaveweapon = true;
	player->weaponowned[weapon] = true;
	player->pendingweapon = weapon;
    }
	
    return (gaveweapon || gaveammo);
}

 

//
// P_GiveBody
// Returns false if the body isn't needed at all
//
boolean
P_GiveBody
( player_t*	player,
  int		num )
{
    if (player->health >= MAXHEALTH)
	return false;
		
    player->health += num;
    if (player->health > MAXHEALTH)
	player->health = MAXHEALTH;
    player->mo->health = player->health;
	
    return true;
}



//
// P_GiveArmor
// Returns false if the armor is worse
// than the current armor.
//
boolean
P_GiveArmor
( player_t*	player,
  int		armortype )
{
    int		hits;
	
    hits = armortype*100;
    if (player->armorpoints >= hits)
	return false;	// don't pick up
		
    player->armortype = armortype;
    player->armorpoints = hits;
	
    return true;
}



//
// P_GiveCard
//
void
P_GiveCard
( player_t*	player,
  card_t	card )
{
    if (player->cards[card])
	return;
    
    player->bonuscount += netgame ? BONUSADD : 0; // [crispy] Fix "Key pickup resets palette"
    player->cards[card] = 1;

    if (BotsInGame)
        SendKeycardToBots();   // [marshmallow] Share any keys we find with all bots
}


//
// P_GivePower
//
boolean
P_GivePower
( player_t*	player,
  int /*powertype_t*/	power )
{
    if (power == pw_invulnerability)
    {
	player->powers[power] = INVULNTICS;
	return true;
    }
    
    if (power == pw_invisibility)
    {
	player->powers[power] = INVISTICS;
	player->mo->flags |= MF_SHADOW;
	return true;
    }
    
    if (power == pw_infrared)
    {
	player->powers[power] = INFRATICS;
	return true;
    }
    
    if (power == pw_ironfeet)
    {
	player->powers[power] = IRONTICS;
	return true;
    }
    
    if (power == pw_strength)
    {
    if (!Marshmallow_SaveItems)  // [marshmallow] Don't give health immediately since it's a portable medkit
	    P_GiveBody (player, 100);
	player->powers[power] = 1;
	return true;
    }
	
    if (player->powers[power])
	return false;	// already got it
		
    player->powers[power] = 1;
    return true;
}



//
// P_TouchSpecialThing
//
void
P_TouchSpecialThing
( mobj_t*	special,
  mobj_t*	toucher )
{
    player_t*	player;
    int		i;
    fixed_t	delta;
    int		sound;
    const boolean dropped = ((special->flags & MF_DROPPED) != 0);
		
    delta = special->z - toucher->z;

    if (delta > toucher->height
	|| delta < -8*FRACUNIT)
    {
	// out of reach
	return;
    }

    // [marshmallow] Bots don't pick up any items
    if ( IsBot(toucher->player) )
    {
        if (special->sprite == SPR_BAR1)
        {
            toucher->player->touching_barrel = true;
            toucher->player->current_barrel = special;
            toucher->player->barrel_timeout = 30;
        }

        return;
    }
    // [m]
	
    sound = sfx_itemup;	
    player = toucher->player;

    // Dead thing touching.
    // Can happen with a sliding player corpse.
    if (toucher->health <= 0)
	return;

    // Identify by sprite.
    switch (special->sprite)
    {
      // [marshmallow] for pushing barrels
      case SPR_BAR1:
          if ( !Marshmallow_BarrelPushing )
              return;

          toucher->player->touching_barrel = true;
          toucher->player->current_barrel = special;
          toucher->player->barrel_timeout = 30;

          return;

      case SPR_BEXP:
          return;
      // [m]

            // armor
      case SPR_ARM1:

    // [marshmallow] If we don't need it, don't pick it up
    if (Marshmallow_ConservePowerups && player->armorpoints >= 200)
        return;

    if (!P_GiveArmor (player, deh_green_armor_class))
	    return;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTARMOR);
	break;
		
      case SPR_ARM2:

    // [marshmallow] If we don't need it, don't pick it up
    if (Marshmallow_ConservePowerups && player->armorpoints >= 200)
        return;

	if (!P_GiveArmor (player, deh_blue_armor_class))
	    return;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTMEGA);
	break;
	
	// bonus items
      case SPR_BON1:

    // [marshmallow] If we don't need it, don't pick it up
    if (Marshmallow_ConservePowerups && player->health >= 200)
        return;

	player->health++;		// can go over 100%
	if (player->health > deh_max_health)
	    player->health = deh_max_health;
	player->mo->health = player->health;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTHTHBONUS);
	break;
	
      case SPR_BON2:

    // [marshmallow] If we don't need it, don't pick it up
    if (Marshmallow_ConservePowerups && player->armorpoints >= 200)
        return;

	player->armorpoints++;		// can go over 100%
	if (player->armorpoints > deh_max_armor && gameversion > exe_doom_1_2)
	    player->armorpoints = deh_max_armor;
        // deh_green_armor_class only applies to the green armor shirt;
        // for the armor helmets, armortype 1 is always used.
	if (!player->armortype)
	    player->armortype = 1;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTARMBONUS);
	break;
	
      case SPR_SOUL:

      // [marshmallow]
      if (Marshmallow_ConservePowerups)
      {
          int playerwillget;
          int newremaining;

          if (player->health >= 200)
              return;  // leave the item

          if (!special->remaining)      // If not touched yet, init to full capacity
              special->remaining = 100;

          toucher->player->message = DEH_String(SOULSPHEREHEALS);
          S_StartSound(special, sfx_getpow);
          player->bonuscount += BONUSADD;

          if (player->health + special->remaining <= 200)
          {
              player->health += special->remaining;
              player->mo->health += special->remaining;

              break;  //  Do P_RemoveMobj()
          }

          if (player->health + special->remaining > 200)
          {
              playerwillget = player->health + special->remaining - 200;

              player->health = 200;
              player->mo->health = 200;

              newremaining = special->remaining - playerwillget;

              special->remaining = newremaining;

              if (special->remaining <= 0)
                  break;  //  Do P_RemoveMobj()
              else
                  return; // leave the item
          }
      }
      // [m]

	player->health += deh_soulsphere_health;
	if (player->health > deh_max_soulsphere)
	    player->health = deh_max_soulsphere;
	player->mo->health = player->health;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTSUPER);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
      case SPR_MEGA:
	if (gamemode != commercial)
	    return;

	// [marshmallow]
    if (Marshmallow_ConservePowerups)
    {
        int playerwillget;

        if (player->health >= 200 &&
            player->armorpoints >= 200)
            return;  // leave the item if full armor and health

        if (!special->remaining &&
            !special->armor)      // If not touched yet, init to full capacity
        {
            special->remaining = 200;
            special->armor = 200;
        }

        toucher->player->message = DEH_String(MEGAHEALS);
        S_StartSound(special, sfx_getpow);

        player->bonuscount += BONUSADD;

        if (player->health < 200 && special->remaining > 0)
        {
            playerwillget = (200 - player->health);
            if (special->remaining < playerwillget)
                playerwillget = special->remaining;

            player->health+=playerwillget;
            player->mo->health+=playerwillget;

            special->remaining -= playerwillget;
        }

        if (player->armorpoints < 200 && special->armor > 0)
        {
            playerwillget = (200 - player->armorpoints);
            if (special->armor < playerwillget)
                playerwillget = special->armor;

            player->armorpoints+=playerwillget;

            special->armor -= playerwillget;
        }

        if (special->remaining <= 0 && special->armor <= 0)
            break;  //  Do P_RemoveMobj()
        else
            return; // leave the item
    }
    // [m]

	player->health = deh_megasphere_health;
	player->mo->health = player->health;
        // We always give armor type 2 for the megasphere; dehacked only 
        // affects the MegaArmor.
	P_GiveArmor (player, 2);

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTMSPHERE);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
	// cards
	// leave cards for everyone
      case SPR_BKEY:
	if (!player->cards[it_bluecard])
	    player->message = DEH_String(GOTBLUECARD);
	P_GiveCard (player, it_bluecard);
	if (!netgame)
	    break;
	return;
	
      case SPR_YKEY:
	if (!player->cards[it_yellowcard])
	    player->message = DEH_String(GOTYELWCARD);
	P_GiveCard (player, it_yellowcard);
	if (!netgame)
	    break;
	return;
	
      case SPR_RKEY:
	if (!player->cards[it_redcard])
	    player->message = DEH_String(GOTREDCARD);
	P_GiveCard (player, it_redcard);
	if (!netgame)
	    break;
	return;
	
      case SPR_BSKU:
          // [marshmallow]
          if (special->flags & MF_TREASURE)
          {
              CollectTreasure(BLUE, toucher);
              break;
          }
          // [m]

	if (!player->cards[it_blueskull])
	    player->message = DEH_String(GOTBLUESKUL);
	P_GiveCard (player, it_blueskull);
	if (!netgame)
	    break;
	return;
	
      case SPR_YSKU:
          // [marshmallow]
          if (special->flags & MF_TREASURE)
          {
              CollectTreasure(GOLD, toucher);
              break;
          }
            // [m]

            if (!player->cards[it_yellowskull])
	    player->message = DEH_String(GOTYELWSKUL);
	P_GiveCard (player, it_yellowskull);
	if (!netgame)
	    break;
	return;
	
      case SPR_RSKU:
          // [marshmallow]
          if (special->flags & MF_TREASURE)
          {
              CollectTreasure(RED, toucher);

              if ( GetGameType() != DOOM1 )
                  sound = sfx_keenpn;

              break;
          }
            // [m]

            if (!player->cards[it_redskull])
	    player->message = DEH_String(GOTREDSKULL);
	P_GiveCard (player, it_redskull);
	if (!netgame)
	    break;
	return;
	
	// medikits, heals
      case SPR_STIM:
	if (!P_GiveBody (player, 10))
	    return;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTSTIM);
	break;
	
      case SPR_MEDI:

      // [marshmallow]
      if (Marshmallow_ConservePowerups)
      {
          int used;
          int newremaining;

          if (!special->remaining)
              special->remaining = 25;

          if (toucher->health >= 100)
              return;

          if (toucher->health <= 100 - special->remaining)
          {
              P_GiveBody (player, special->remaining);

              if (Marshmallow_PickupMessages)
              {
                  player->message = DEH_String(USEDMEDICAL);
                  player->bonuscount += BONUSADD;
              }

              break;
          }

          newremaining = toucher->health + special->remaining - 100;
          used = special->remaining - newremaining;

          player->health += used;
          player->mo->health += used;

          player->message = DEH_String(USEDMEDICAL);

          player->bonuscount += BONUSADD;

          if (newremaining <= 0)
              break;	 // Do P_RemoveMobj()
          else
              special->remaining = newremaining;

          return;
      }
      // [m]

	if (!P_GiveBody (player, 25))
	    return;

	// [crispy] show "Picked up a Medikit that you really need" message as intended
	if (player->health < 50)
	    player->message = DEH_String(GOTMEDINEED);
	else
	    player->message = DEH_String(GOTMEDIKIT);
	break;

	
	// power ups
      case SPR_PINV:

      // [marshmallow]
      if (Marshmallow_SaveItems)
      {
          if (toucher->player->extra_powers[ITEM_INVUL])
          {
              player->message = DEH_String(HAVEINVUL);
              return;
          }

          toucher->player->extra_powers[ITEM_INVUL] = true;

          player->message = DEH_String(ADDEDINVUL);
          sound = sfx_getpow;

          if (invmenu_on)
              invmenu_selection = INVUL_SELECTED;

          break;
      }
      // [m]

	if (!P_GivePower (player, pw_invulnerability))
	    return;

	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTINVUL);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
      case SPR_PSTR:

      // [marshmallow]
      if (Marshmallow_SaveItems)
      {
          if (toucher->player->extra_powers[ITEM_MEDKIT])
          {
              if (!player->powers[pw_strength])
              {
                  P_GivePower (player, pw_strength);  // Give berserk anyway if we don't have it
                  if (Marshmallow_BerserkAutoSwitch)
                  {
                      if (player->readyweapon != wp_fist)
                          player->pendingweapon = wp_fist;
                  }
              }

              return;  // Avoid doing P_RemoveMobj() so we can leave item on the ground
          }

          toucher->player->extra_powers[ITEM_MEDKIT] = true;
          toucher->player->medkit_remaining = 100;

          player->message = DEH_String(ADDEDMEDKIT);
          sound = sfx_getpow;

          if (invmenu_on)
              invmenu_selection = MEDKIT_SELECTED;

          P_GivePower (player, pw_strength);  // Give regular berserk in addition to the portable medkit
          if (Marshmallow_BerserkAutoSwitch)
          {
              if (player->readyweapon != wp_fist)
                  player->pendingweapon = wp_fist;
          }
          break;
      }
      // [m]

	if (!P_GivePower (player, pw_strength))
	    return;

    if (!Marshmallow_SaveItems && Marshmallow_PickupMessages)  // [marshmallow]
	    player->message = DEH_String(GOTBERSERK);

    if (Marshmallow_BerserkAutoSwitch)	// [marshmallow]
	if (player->readyweapon != wp_fist)
	    player->pendingweapon = wp_fist;
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
      case SPR_PINS:

      // [marshmallow]
      if (special->flags & MF_DROPPED && toucher->player)
      {
          if (Marshmallow_ConservePowerups)
          {
              int playerwillget;
              int newremaining;

              if (player->health >= 200
                  || player->mo->health >= 200)
                  return;  // leave the item

              toucher->player->message = DEH_String(DEMONSPHEREHEALS);
              S_StartSound(special, sfx_getpow);
              player->bonuscount += BONUSADD;

              if (player->health + special->remaining <= 200)
              {
                  player->health += special->remaining;
                  player->mo->health += special->remaining;

                  break;  //  Do P_RemoveMobj()
              }

              if (player->health + special->remaining > 200)
              {
                  playerwillget = player->health + special->remaining - 200;

                  player->health = 200;
                  player->mo->health = 200;

                  newremaining = special->remaining - playerwillget;

                  special->remaining = newremaining;

                  if (special->remaining <= 0)
                      break;  //  Do P_RemoveMobj()
                  else
                      return; // leave the item
              }
          }

          if ( !Marshmallow_GiveNewInvisPowerup(toucher, special) )
              return;  // leave the item
          else
              break;

          break;
      }

      if (Marshmallow_SaveItems)
      {
          if (toucher->player->extra_powers[ITEM_INVIS])
          {
              player->message = DEH_String(HAVEINVIS);
              return;
          }

          toucher->player->extra_powers[ITEM_INVIS] = true;

          player->message = DEH_String(ADDEDINVIS);
          sound = sfx_getpow;

          if (invmenu_on)
              invmenu_selection = INVIS_SELECTED;

          break;
      }
      // [m]

	if (!P_GivePower (player, pw_invisibility))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTINVIS);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
      case SPR_SUIT:

      // [marshmallow]
      if (Marshmallow_SaveItems)
      {
          if (toucher->player->extra_powers[ITEM_RADSUIT])
          {
              player->message = DEH_String(HAVERADSUIT);
              return;
          }

          toucher->player->extra_powers[ITEM_RADSUIT] = true;

          player->message = DEH_String(ADDEDRADSUIT);
          sound = sfx_getpow;

          if (invmenu_on)
              invmenu_selection = RADSUIT_SELECTED;

          break;
      }
      // [m]

	if (!P_GivePower (player, pw_ironfeet))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTSUIT);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
      case SPR_PMAP:
	if (!P_GivePower (player, pw_allmap))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTMAP);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
      case SPR_PVIS:

      // [marshmallow]
      if (Marshmallow_SaveItems)
      {
          if (toucher->player->extra_powers[ITEM_VISOR])
          {
              player->message = DEH_String(HAVEVISOR);
              return;
          }

          toucher->player->extra_powers[ITEM_VISOR] = true;

          player->message = DEH_String(ADDEDVISOR);
          sound = sfx_getpow;

          if (invmenu_on)
              invmenu_selection = VISOR_SELECTED;

          break;
      }
      // [m]

	if (!P_GivePower (player, pw_infrared))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTVISOR);
	if (gameversion > exe_doom_1_2)
	    sound = sfx_getpow;
	break;
	
	// ammo
	// [NS] Give half ammo for drops of all types.
      case SPR_CLIP:
	/*
	if (special->flags & MF_DROPPED)
	{
	    if (!P_GiveAmmo (player,am_clip,0))
		return;
	}
	else
	{
	    if (!P_GiveAmmo (player,am_clip,1))
		return;
	}
	*/
	    if (!P_GiveAmmo (player,am_clip,1,dropped))
		return;
	    if (Marshmallow_PickupMessages)   // [marshmallow]
	        player->message = DEH_String(GOTCLIP);
	break;
	
      case SPR_AMMO:

      // [marshmallow]
      if (Marshmallow_ConservePowerups)
      {
          int playerwillget;
          int newremaining;

          if (player->ammo[am_clip] >= player->maxammo[am_clip])
              return;  // leave the item

          if (!special->remaining)
              special->remaining = 50;

          if (Marshmallow_PickupMessages)
              toucher->player->message = DEH_String(FOUNDBULL);

          player->bonuscount += BONUSADD;

          if (player->ammo[am_clip] + special->remaining > player->maxammo[am_clip])
          {
              playerwillget = player->maxammo[am_clip] - player->ammo[am_clip];

              player->ammo[am_clip] = player->maxammo[am_clip];

              newremaining = special->remaining - playerwillget;

              special->remaining = newremaining;

              if (special->remaining <= 0)
                  break;  //  do P_RemoveMobj()
              else
                  return; // leave the item
          }
      }
      // [m]

	if (!P_GiveAmmo (player, am_clip,5,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTCLIPBOX);
	break;
	
      case SPR_ROCK:
	if (!P_GiveAmmo (player, am_misl,1,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTROCKET);
	break;
	
      case SPR_BROK:

      // [marshmallow]
      if (Marshmallow_ConservePowerups)
      {
          int playerwillget;
          int newremaining;

          if (player->ammo[am_misl] >= player->maxammo[am_misl])
              return;  // leave the item

          if (!special->remaining)
              special->remaining = 5;

          if (Marshmallow_PickupMessages)
              toucher->player->message = DEH_String(FOUNDROCK);

          player->bonuscount += BONUSADD;

          playerwillget = player->maxammo[am_misl] - player->ammo[am_misl];

          if (player->ammo[am_misl] + special->remaining > player->maxammo[am_misl])
          {
              player->ammo[am_misl] = player->maxammo[am_misl];

              newremaining = special->remaining - playerwillget;

              special->remaining = newremaining;

              if (special->remaining <= 0)
                  break;  //  Do P_RemoveMobj()
              else
                  return; // leave the item
          }
      }
      // [m]

	if (!P_GiveAmmo (player, am_misl,5,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTROCKBOX);
	break;
	
      case SPR_CELL:
	if (!P_GiveAmmo (player, am_cell,1,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTCELL);
	break;
	
      case SPR_CELP:

      // [marshmallow]
      if (Marshmallow_ConservePowerups)
      {
          int playerwillget;
          int newremaining;

          if (player->ammo[am_cell] >= player->maxammo[am_cell])
              return;  // leave the item

          if (!special->remaining)
              special->remaining = 100;

          if (Marshmallow_PickupMessages)
              toucher->player->message = DEH_String(FOUNDCELL);

          player->bonuscount += BONUSADD;

          if (player->ammo[am_cell] + special->remaining > player->maxammo[am_cell])
          {
              playerwillget = player->maxammo[am_cell] - player->ammo[am_cell];

              player->ammo[am_cell] = player->maxammo[am_cell];

              newremaining = special->remaining - playerwillget;

              special->remaining = newremaining;

              if (special->remaining <= 0)
                  break;  // Do P_RemoveMobj()
              else
                  return; // leave the item
          }
      }
      // [m]

	if (!P_GiveAmmo (player, am_cell,5,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTCELLBOX);
	break;
	
      case SPR_SHEL:
	if (!P_GiveAmmo (player, am_shell,1,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTSHELLS);
	break;
	
      case SPR_SBOX:

      // [marshmallow]
      if (Marshmallow_ConservePowerups)
      {
          int playerwillget;
          int newremaining;

          if (player->ammo[am_shell] >= player->maxammo[am_shell])
              return;  // leave the item

          if (!special->remaining)
              special->remaining = 20;

          if (Marshmallow_PickupMessages)
              toucher->player->message = DEH_String(FOUNDBUCK);

          player->bonuscount += BONUSADD;

          playerwillget = player->maxammo[am_shell] - player->ammo[am_shell];

          if (player->ammo[am_shell] + special->remaining > player->maxammo[am_shell])
          {
              player->ammo[am_shell] = player->maxammo[am_shell];

              newremaining = special->remaining - playerwillget;

              special->remaining = newremaining;

              if (special->remaining <= 0)
                  break;  // Do P_RemoveMobj()
              else
                  return; // leave the item
          }
      }
      // [m]

	if (!P_GiveAmmo (player, am_shell,5,dropped))
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTSHELLBOX);
	break;
	
      case SPR_BPAK:

    // [marshmallow] If this was a backpack with the MF_DROPPED flag, use our new inventory backpack functionality
    if (special->flags & MF_DROPPED && toucher->player->playerstate == PST_LIVE )
    {
        if ( deathmatch || special->is_gift )
        {
            if (special->owner == toucher->player->player_number && !deathmatch)
                return;

            if ( !Marshmallow_GimmeThatPhatLoot(toucher, special) )  // Basic ammo bonus backpack
            {
                toucher->player->message = "YOU CANNOT CARRY ANY MORE AMMO.";
                return;  // Ammo is full, so leave it on the ground
            }
        }
        else
        {
            if ( special->owner != toucher->player->player_number )  // Only the player that dropped it can pick it up
                return;

            RecoverInventoryFromBackpack(toucher, special);  // Entire player inventory backpack
        }
    }
    // [m]

	if (!player->backpack)
	{
	    for (i=0 ; i<NUMAMMO ; i++)
		player->maxammo[i] *= 2;
	    player->backpack = true;
	}
	for (i=0 ; i<NUMAMMO ; i++)
	    P_GiveAmmo (player, i, 1, false);
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTBACKPACK);
	break;
	
	// weapons
	// [NS] Give half ammo for all dropped weapons.
      case SPR_BFUG:
	if (!P_GiveWeapon (player, wp_bfg, dropped) )
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTBFG9000);
	sound = sfx_wpnup;	
	break;
	
      case SPR_MGUN:
        if (!P_GiveWeapon(player, wp_chaingun,
                          (special->flags & MF_DROPPED) != 0))
            return;
        if (Marshmallow_PickupMessages)   // [marshmallow]
	        player->message = DEH_String(GOTCHAINGUN);
	sound = sfx_wpnup;	
	break;
	
      case SPR_CSAW:
	if (!P_GiveWeapon (player, wp_chainsaw, dropped) )
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTCHAINSAW);
	sound = sfx_wpnup;	
	break;
	
      case SPR_LAUN:
	if (!P_GiveWeapon (player, wp_missile, dropped) )
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTLAUNCHER);
	sound = sfx_wpnup;	
	break;
	
      case SPR_PLAS:
	if (!P_GiveWeapon (player, wp_plasma, dropped) )
	    return;
	if (Marshmallow_PickupMessages)   // [marshmallow]
	    player->message = DEH_String(GOTPLASMA);
	sound = sfx_wpnup;	
	break;
	
      case SPR_SHOT:
        if (!P_GiveWeapon(player, wp_shotgun,
                          (special->flags & MF_DROPPED) != 0))
            return;
        if (Marshmallow_PickupMessages)   // [marshmallow]
	        player->message = DEH_String(GOTSHOTGUN);
	sound = sfx_wpnup;	
	break;
		
      case SPR_SGN2:
        if (!P_GiveWeapon(player, wp_supershotgun,
                          (special->flags & MF_DROPPED) != 0))
            return;
        if (Marshmallow_PickupMessages)   // [marshmallow]
	        player->message = DEH_String(GOTSHOTGUN2);
	sound = sfx_wpnup;	
	break;
		
      default:
	I_Error ("P_SpecialThing: Unknown gettable thing");
    }

    // [marshmallow] WeaponsStay option should not apply to dropped weapons
    if (IsWeapon(special) && Marshmallow_WeaponsStay && (!special->flags & MF_DROPPED))
        return;
    // [m]
	
    if (special->flags & MF_COUNTITEM
        && (!special->flags & MF_DROPPED))  // [marshmallow] Don't count dropped items towards intermission stats
	player->itemcount++;
    P_RemoveMobj (special);
    player->bonuscount += BONUSADD;

    // [marshmallow]
    if ( !treasure_bag.all_collected
         && treasure_bag.remaining_in_level == 0
         && treasure_bag.total_in_level > 0 )
    {
        if (GetGameType() == DOOM1)
            sound = sfx_getpow;
        else
            sound = sfx_keendt;

        treasure_bag.all_collected = true;
    }
    // [m]

    if (player == &players[consoleplayer])
	S_StartSound (NULL, sound);
}


//
// KillMobj
//
void
P_KillMobj
( mobj_t*	source,
  mobj_t*	target )
{
    mobjtype_t	item;
    mobj_t*	mo;

    // [marshmallow] For gibs
    int negative_spawnhealth = target->info->spawnhealth * -1;
    gibmode_t gibmode = Marshmallow_GibMode;

    // [marshmallow] Kill stuff
    if (Marshmallow_Sandbox)
    {
        if ( IsMonster(target)
             && target->type != MT_BARREL
             && target->type != MT_KEEN)
            sandbox.count--;
    }

    if ( IsMonster(target) )
    {
        if ( IsPlayer(source) )
            source->player->victim = NULL;

        PKE_KillEntity(target);

        if (source &&
            source->type == MT_PLAYER )
        {
            if ( !deathmatch )
                level_stats.squad_kills++;

            if ( !IsBot(source->player) )
                level_stats.monsters_killed++;
        }
    }

    if ( deathmatch && IsPlayer(target) )  // [marshmallow] Count deathmatch frags for profile stats
        level_stats.dm_frags++;
    // [m]
	
    target->flags &= ~(MF_SHOOTABLE|MF_FLOAT|MF_SKULLFLY);

    if (target->type != MT_SKULL)
	target->flags &= ~MF_NOGRAVITY;

    target->flags |= MF_CORPSE|MF_DROPOFF;
    target->height >>= 2;

    // [marshmallow]
    AnnounceWhoKilledWhat(source, target, players);
    AnnounceMostDangerousMonstersDeath(target);
    // [m]

    if (source && source->player)
    {
	// count for intermission
	if (target->flags & MF_COUNTKILL)
	    source->player->killcount++;	

	if (target->player)
	    source->player->frags[target->player-players]++;
    }
    else if (!netgame && (target->flags & MF_COUNTKILL) )
    {
	// count all monster deaths,
	// even those caused by other monsters
	players[0].killcount++;
    }
    
    if (target->player)
    {
	// count environment kills against you
	if (!source)	
	    target->player->frags[target->player-players]++;
			
	target->flags &= ~MF_SOLID;
	target->player->playerstate = PST_DEAD;
	P_DropWeapon (target->player);
	// [crispy] center view when dying
	target->player->centering = true;
	// [JN] & [crispy] Reset the yellow bonus palette when the player dies
	target->player->bonuscount = 0;
	// [JN] & [crispy] Remove the effect of the inverted palette when the player dies
	target->player->fixedcolormap = target->player->powers[pw_infrared] ? 1 : 0;

	if (target->player == &players[consoleplayer]
	    && automapactive)
	{
	    // don't die in auto map,
	    // switch view prior to dying
	    AM_Stop ();
	}
	
    }

    // [crispy] Lost Soul, Pain Elemental and Barrel explosions are translucent
    if (target->type == MT_SKULL ||
        target->type == MT_PAIN ||
        target->type == MT_BARREL)
        target->flags |= MF_TRANSLUCENT;

    // [marshmallow]
    if (gibmode == BRUTAL_GIBS)
        negative_spawnhealth = negative_spawnhealth / 2;  // Make xdeath animation more probable if we are in BRUTAL mode

    if (IsPlayer(source) && source->player->readyweapon == wp_pistol) // If we're using pistol, don't do any gib stuff
    {
        P_SetMobjState (target, target->info->deathstate);
    }
    else
    {
        if (target->health < negative_spawnhealth
            && target->info->xdeathstate)
        {
            if (gibmode == BRUTAL_GIBS
                && target->type != MT_SKULL)
            {
                BrutalSplat(target);
            }

            P_SetMobjState (target, target->info->xdeathstate);
        }
        else
        {
            P_SetMobjState (target, target->info->deathstate);
        }

        GibFactory(target, source);
    }
    // [m]

    // [crispy] randomly flip corpse, blood and death animation sprites
    if (target->flags & MF_FLIPPABLE)
    {
	target->health = (target->health & (int)~1) - (Crispy_Random() & 1);
    }

    if (target->tics < 1)
	target->tics = 1;
		
    //	I_StartSound (&actor->r, actor->info->deathsound);

    // In Chex Quest, monsters don't drop items.

    if (gameversion == exe_chex)
    {
        return;
    }

    // [marshmallow] Drop Goodies
    if (Marshmallow_DropGoodies)
    {
        if (!IsPlayer(source) && IsMonster(target))
            return;  // Don't drop anything if player wasn't the killer
        else
            Marshmallow_NewDropItemsRoutine(target);

        return;
    }
    // [m]

    // Drop stuff.
    // This determines the kind of object spawned
    // during the death frame of a thing.
    switch (target->type)
    {
      case MT_WOLFSS:
      case MT_POSSESSED:
	item = MT_CLIP;
	break;
	
      case MT_SHOTGUY:
	item = MT_SHOTGUN;
	break;
	
      case MT_CHAINGUY:
	item = MT_CHAINGUN;
	break;

	// [marshmallow] We need this here in case Drop Goodies is off and Drop Backpack is on
	case MT_PLAYER:
        if (Marshmallow_DropBackpack)
        {
            if ( !deathmatch )
            {
                DropInventoryInBackpack(target);
            }
            else
            {
                CreateBackpack(target, false);
                DropWeaponOnPlayerDeath(target);
            }
        }
        return;
    // [m]
	
      default:
	return;
    }

    mo = P_SpawnMobj (target->x,target->y,ONFLOORZ, item);
    mo->flags |= MF_DROPPED;	// special versions of items
}




//
// P_DamageMobj
// Damages both enemies and players
// "inflictor" is the thing that caused the damage
//  creature or missile, can be NULL (slime, etc)
// "source" is the thing to target after taking damage
//  creature or NULL
// Source and inflictor are the same for melee attacks.
// Source can be NULL for slime, barrel explosions
// and other environmental stuff.
//
void
P_DamageMobj
( mobj_t*	target,
  mobj_t*	inflictor,
  mobj_t*	source,
  int 		damage )
{
    unsigned	ang;
    int		saved;
    player_t*	player;
    fixed_t	thrust;
    int		temp;
	
    if ( !(target->flags & MF_SHOOTABLE) )
	return;	// shouldn't happen...
		
    if (target->health <= 0)
	return;

    if (sandbox.design_mode)  // [marshmallow] Don't take environment damage while designing a sandbox
        return;

    if ( target->flags & MF_SKULLFLY )
    {
	target->momx = target->momy = target->momz = 0;
    }
	
    player = target->player;
    if (player && gameskill == sk_baby)
	damage >>= 1; 	// take half damage in trainer mode
		

    // Some close combat weapons should not
    // inflict thrust and push the victim out of reach,
    // thus kick away unless using the chainsaw.
    if (inflictor
	&& !(target->flags & MF_NOCLIP)
	&& (!source
	    || !source->player
	    || source->player->readyweapon != wp_chainsaw))
    {
	ang = R_PointToAngle2 ( inflictor->x,
				inflictor->y,
				target->x,
				target->y);
		
	thrust = damage*(FRACUNIT>>3)*100/target->info->mass;

    if ( physics_mode && !IsPlayer(target) )
        thrust *= thrust_multiplier[ physics_mode ];      // [marshmallow] Increasing thrust/inertia

	// make fall forwards sometimes
	if ( damage < 40
	     && damage > target->health
	     && target->z - inflictor->z > 64*FRACUNIT
	     && (P_Random ()&1)
	     && physics_mode )  // [marshmallow] Don't fall forward when using alternate physics modes
    {
	    ang += ANG180;
	    thrust *= 4;
	}
		
	ang >>= ANGLETOFINESHIFT;
	target->momx += FixedMul (thrust, finecosine[ang]);
	target->momy += FixedMul (thrust, finesine[ang]);
    }

    SetPlayerTarget(source, target);  // [marshmallow]
    
    // player specific
    if (player)
    {

    // [marshmallow]
    if (Marshmallow_MirrorDamage)
        Marshmallow_DoMirrorDamage(source, target);

    if (!CheckPlayerDamage(source, target))  // [marshmallow] Returns true if we're doing damage, false if we're not
        return;
    // [m]

        // end of game hell hack
	if (target->subsector->sector->special == 11
	    && damage >= target->health)
	{
	    damage = target->health - 1;
	}
	

	// Below certain threshold,
	// ignore damage in GOD mode, or with INVUL power.
	if ( damage < 1000
	     && ( (player->cheats&CF_GODMODE)
		  || player->powers[pw_invulnerability] ) )
	{
	    return;
	}
	
	if (player->armortype)
	{
	    if (player->armortype == 1)
		saved = damage/3;
	    else
		saved = damage/2;
	    
	    if (player->armorpoints <= saved)
	    {
		// armor is used up
		saved = player->armorpoints;
		player->armortype = 0;
	    }
	    player->armorpoints -= saved;
	    damage -= saved;
	}
	player->health -= damage; 	// mirror mobj health here for Dave
	// [crispy] negative player health
	player->neghealth = player->health;
	if (player->neghealth < -99)
	    player->neghealth = -99;
	if (player->health < 0)
	    player->health = 0;
	
	player->attacker = source;
	player->damagecount += damage;	// add damage after armor / invuln

	if (player->damagecount > 100)
	    player->damagecount = 100;	// teleport stomp does 10k points...
	
	temp = damage < 100 ? damage : 100;

	if (player == &players[consoleplayer])
	    I_Tactile (40,10,40+temp*2);
    }

    RegisterShotsHit(inflictor, target);  // [marshmallow]

    // do the damage	
    target->health -= damage;	
    if (target->health <= 0)
    {
	P_KillMobj (source, target);
	return;
    }

    if ( (P_Random () < target->info->painchance)
	 && !(target->flags&MF_SKULLFLY) )
    {
	target->flags |= MF_JUSTHIT;	// fight back!
	
	P_SetMobjState (target, target->info->painstate);
    }

    // [marshmallow] If target is a bot, don't change its target; this is handled in our own bot AI functions
    if (BotsInGame)
    {
        int	i;

        for (i=BOT_1;i<MAX_BOTS;i++)
            if (players[i].mo == target)
                return;
    }
    // [m]
			
    target->reactiontime = 0;		// we're awake now...	

    if ( (!target->threshold || target->type == MT_VILE)
	 && source && (source != target || gameversion <= exe_doom_1_2)
	 && source->type != MT_VILE)
    {
	// if not intent on another player,
	// chase after this one
	target->target = source;
	target->threshold = BASETHRESHOLD;
	if (target->state == &states[target->info->spawnstate]
	    && target->info->seestate != S_NULL)
	    P_SetMobjState (target, target->info->seestate);
    }
			
}

