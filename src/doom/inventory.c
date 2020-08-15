#include "marshmallow.h"


void UsePortableMedkit()
{
	int playerwillget, newremaining;
	//player_t player = players[consoleplayer];

	if (MAIN_PLAYER.health >= 100
		|| MAIN_PLAYER.mo->health >= 100)
	{
		MAIN_PLAYER.message = DEH_String(ALREADYFULL);
		return;
	}

	if (MAIN_PLAYER.playerstate != PST_LIVE)
	return;
					
	playerwillget = 100 - MAIN_PLAYER.health;  

	if (MAIN_PLAYER.medkit_remaining <=	playerwillget)	// TODO: go to p_inter.c and go to every SaveItems block and make sure this is <=
	{  // if we don't have enough to heal to 100 OR we have exactly enough, just give what's left and set remaining to zero
		MAIN_PLAYER.health += MAIN_PLAYER.medkit_remaining;
		MAIN_PLAYER.mo->health += MAIN_PLAYER.medkit_remaining;
		MAIN_PLAYER.medkit_remaining = 0;
		MAIN_PLAYER.extra_powers[PORTABLE_MEDKIT] = false;	
	}
	else
	{  // if we have more than we need, take what we need and set new remaining amount
		MAIN_PLAYER.health += playerwillget;
		MAIN_PLAYER.mo->health += playerwillget;
		newremaining = MAIN_PLAYER.medkit_remaining - playerwillget;
		MAIN_PLAYER.medkit_remaining = newremaining;
	}
					
	MAIN_PLAYER.message = DEH_String(USINGMEDKIT);
	
	invmenu_on = false;
	offer_medkit = false;
	offertimeout_medkit = 0;

	MAIN_PLAYER.bonuscount += BONUSADD;  // HACK... just for screen flash...
}


int free_respawn_ammo[NUMAMMO] = { RESPAWN_BULLETS, RESPAWN_SHELLS, RESPAWN_CELLS, RESPAWN_ROCKETS };

void RecoverInventoryFromBackpack(mobj_t* toucher, mobj_t* special)
{
	int i;

	mobj_t* mo = special;

	// Recover special items
	for (i=0; i<MAX_INV_ITEMS; i++)
	{
		if ( mo->dropped_backpack.items[i] )
		{
			toucher->player->extra_powers[i] = true;

			if (i == ITEM_MEDKIT)
				toucher->player->medkit_remaining = 100;
		}
	}

	// Recover weapons
	for (i=0; i<NUMWEAPONS; i++)
	{
		if ( mo->dropped_backpack.weapons[i] )
			toucher->player->weaponowned[i] = true;
	}

	// Backpack powerup yes/no
	if ( mo->dropped_backpack.backpack && !toucher->player->backpack)
	{
		toucher->player->backpack = true;	
		
		for (i=0 ; i<NUMAMMO ; i++)
			toucher->player->maxammo[i] *= 2;
	}
	
	// Recover ammo
	for (i=0; i<NUMAMMO; i++)
	{
		if ( !mo->dropped_backpack.ammo[i] )
			continue;
		
		toucher->player->ammo[i] += mo->dropped_backpack.ammo[i];

		if ( !Marshmallow_KeepFreeAmmo
			&& toucher->player->ammo[i] - free_respawn_ammo[i] > 0 )  // Remove the free ammo we respawned with
		{
			//if ( mo->dropped_backpack.ammo[i] >= free_respawn_ammo[i] )  // TESTING THIS LINE
			//if ( toucher->player->ammo[i] > free_respawn_ammo[i] * 2 )
				toucher->player->ammo[i] -= free_respawn_ammo[i];
		}

		if ( toucher->player->ammo[i] > toucher->player->maxammo[i] )  // Don't let us go over maxammo
			toucher->player->ammo[i] = toucher->player->maxammo[i];
	}

	toucher->player->message = DEH_String(INVRECOVERED); 
}


boolean Marshmallow_GimmeThatPhatLoot(mobj_t* toucher, mobj_t* special)
{	
	int ammo;
	int ammo_full = 0;

	for (ammo=0; ammo<NUMAMMO; ammo++)
	{
		if ( toucher->player->ammo[ammo] == toucher->player->maxammo[ammo] )
			ammo_full++;
		else
			continue;
	}

	if (ammo_full == NUMAMMO)
		return false;  // need to do true below!

	if (!deathmatch)
		toucher->player->message = DEH_String(FOUNDBACKPACK);
	  
	P_GiveAmmo (toucher->player, am_clip, Marshmallow_LootMultiplier, false); // TODO: condense all of this to a for loop

	if ( deathmatch )  // In deathmatch, give ammo for all weapons
	{
		P_GiveAmmo (toucher->player, am_shell, Marshmallow_LootMultiplier, false);
		P_GiveAmmo (toucher->player, am_shell, Marshmallow_LootMultiplier, false);
		P_GiveAmmo (toucher->player, am_misl, Marshmallow_LootMultiplier, false);
		P_GiveAmmo (toucher->player, am_cell, Marshmallow_LootMultiplier, false);
		P_GiveAmmo (toucher->player, am_cell, Marshmallow_LootMultiplier, false);

		return true;
	}

	// Otherwise, only give ammo for weapons we have

	if (toucher->player->weaponowned[wp_shotgun])
		P_GiveAmmo (toucher->player, am_shell, Marshmallow_LootMultiplier, false);

	if (toucher->player->weaponowned[wp_supershotgun])
		P_GiveAmmo (toucher->player, am_shell, Marshmallow_LootMultiplier, false);

	if (toucher->player->weaponowned[wp_missile])
		P_GiveAmmo (toucher->player, am_misl, Marshmallow_LootMultiplier, false);

	if (toucher->player->weaponowned[wp_plasma])
		P_GiveAmmo (toucher->player, am_cell, Marshmallow_LootMultiplier, false);

	if (toucher->player->weaponowned[wp_bfg])
		P_GiveAmmo (toucher->player, am_cell, Marshmallow_LootMultiplier, false);

	return true;
}



void DropInventoryInBackpack(mobj_t* target)
{
	int i;

	mobj_t* mo;

	if ( !netgame )
		return;     // never drop a backpack in singleplayer

	if ( IsBot( target->player ) )
		return;    // bots don't drop backpacks

	mo = CreateBackpack(target, true);

	// Save special items (radsuit, invis, etc.)
	for (i=0; i<MAX_INV_ITEMS; i++)
	{
		if ( target->player->extra_powers[i] )
			mo->dropped_backpack.items[i] = true;
	}

	// Save weapons
	for (i=0; i<NUMWEAPONS; i++)
	{
		if ( target->player->weaponowned[i] )
			mo->dropped_backpack.weapons[i] = true;
	}

	// Save ammo
	for (i=0; i<NUMAMMO; i++)
	{
		if ( target->player->ammo[i] )
			mo->dropped_backpack.ammo[i] = target->player->ammo[i];
	}

	// Backpack powerup yes/no
	if ( target->player->backpack )
		mo->dropped_backpack.backpack = true;
}


mobj_t* CreateBackpack(mobj_t* target, boolean is_inventory)
{
	int backpack_x, backpack_y, backpack_z;
	unsigned an;

	mobj_t*	mo;

	int offset;

	if ( is_inventory ) 
		offset = 0;
	else
		offset = MARSHMALLOW_ITEMDROP_OFFSET;

	an = target->angle >> ANGLETOFINESHIFT;

	backpack_x = target->x - FixedMul (24*FRACUNIT, finecosine[an]) - offset; 
	backpack_y = target->y - FixedMul (24*FRACUNIT, finesine[an]) - offset;
	backpack_z = target->z + DROP_FROM_ABOVE_FLOOR;

	mo = P_SpawnMobj (backpack_x, backpack_y, backpack_z, MARSHMALLOW_BACKPACK);
	mo->flags |= MF_DROPPED;

	if ( deathmatch )
		mo->drop_tic = gametic;

	mo->owner = target->player->player_number;

	return mo;
}


void DropBackpackForAFriend(mobj_t* actor)
{
	int x, y;
	unsigned an;
	mobj_t* backpack;
	mobjtype_t item = MARSHMALLOW_BACKPACK;

	if (actor->player->playerstate == PST_DEAD)
	{
		actor->player->message = DEH_String(DEADDROP);
		return;
	}

	if (actor->player->DropObjectDelay)
	{
		if (debugmode)
		actor->player->message = DEH_String(WAITDIPSHIT);
		return;
	}
	else
		actor->player->DropObjectDelay = OBJECT_DROP_DELAY;
	
	if (actor->player->ammo[am_clip] <= 10
		&& actor->player->ammo[am_shell] <= 4
		&& actor->player->ammo[am_misl] <= 1
		&& actor->player->ammo[am_cell] <= 20
		)
	{
		actor->player->message = DEH_String(NODROP);
		return;
	}

	if (actor->player->ammo[am_clip])
	actor->player->ammo[am_clip] -= (MARSHMALLOW_BACKPACK_BULLETS * Marshmallow_LootMultiplier); 

	if (actor->player->ammo[am_shell])
	actor->player->ammo[am_shell] -= (MARSHMALLOW_BACKPACK_SHELLS * Marshmallow_LootMultiplier);    

	if (actor->player->ammo[am_misl])
	actor->player->ammo[am_misl] -= (MARSHMALLOW_BACKPACK_MISSILES * Marshmallow_LootMultiplier);    

	if (actor->player->ammo[am_cell])
	actor->player->ammo[am_cell] -= (MARSHMALLOW_BACKPACK_CELLS * Marshmallow_LootMultiplier);  

	actor->player->message = DEH_String(YOUDROP);

	an = actor->angle >> ANGLETOFINESHIFT; 

	x = actor->x + FixedMul (24*FRACUNIT, finecosine[an])/* + MARSHMALLOW_DROPOBSTACLE_OFFSET_X*/ ; 
	y = actor->y + FixedMul (24*FRACUNIT, finesine[an])/* + MARSHMALLOW_DROPOBSTACLE_OFFSET_Y*/;  

	backpack = P_SpawnMobj (x, y, ONFLOORZ, item);
	backpack->flags |= MF_DROPPED;	

	backpack->is_gift = true;  // NEW
	backpack->owner = actor->player->player_number;  // NEW
}



void StripWeapons(int player)
{
	int weapon, ammo;
	player_t* p;
	p = &players[player];

	for (weapon = wp_shotgun; weapon<NUMWEAPONS; weapon++)  // Start at shotgun so we don't take away pistol
	{
		p->weaponowned[weapon] = false;
		Marshmallow_Arsenal[player][weapon] = false;  // Aug 4 2019 - let's see how this works
	}

	for (ammo = 0; ammo<NUMAMMO; ammo++)
		p->ammo[ammo] = NULL;

	if ( p->backpack )
	{
		p->backpack = false;
	
		for ( ammo=0; ammo<NUMAMMO; ammo++ )
		{
			p->maxammo[ammo] / 2;
		}
	}
	//else???

	p->ammo[am_clip] = deh_initial_bullets; 
	p->readyweapon = wp_pistol;
}


void GiveGradedWeapons(int player)  // TODO: renaming these to WarpWeapons ?  or generally GiveWeapons since it handles fist/pistol
{
	int i;
	player_t* p;
	p = &players[player];

    // Always give this stuff; moved this stuff here from G_PlayerReborn()
    p->weaponowned[wp_fist] = true; 
    p->weaponowned[wp_pistol] = true; 
    p->ammo[am_clip] = deh_initial_bullets; 
	
	if (deathmatch)
	{
		p->readyweapon = p->pendingweapon = wp_pistol;
		return;
	}

	if ( M_CheckParm("-cs") )
	{
		p->weaponowned[wp_chainsaw] = true; 
	}

	if ( M_CheckParm("-sg") )
	{
		p->weaponowned[wp_shotgun] = true; 
		p->ammo[am_shell] += 25;
	}

	if ( M_CheckParm("-ssg") )
	{
		if ( gamemode != commercial && !Marshmallow_WadStealing )  // what about modifiedgame?
		{ 
			p->weaponowned[wp_shotgun] = true; 
			p->ammo[am_shell] += 25;
		}
		else
		{
			p->weaponowned[wp_supershotgun] = true; 
			p->ammo[am_shell] += 25;
		}
	}

	if ( M_CheckParm("-cg") )
	{
		p->weaponowned[wp_chaingun] = true; 
		p->ammo[am_clip] += 100;
	}

	if ( M_CheckParm("-rpg") )
	{
		p->weaponowned[wp_missile] = true; 
		p->ammo[am_misl] += 25;
	}

	if ( M_CheckParm("-pls") )
	{
		p->weaponowned[wp_plasma] = true; 
		p->ammo[am_cell] += 200;
	}

	if ( M_CheckParm("-bfg") )
	{
		p->weaponowned[wp_bfg] = true; 
		p->ammo[am_cell] += 200;
	}

	for (i=0; i<wp_bfg; i++)  // find best weapon we have and set as readyweapon
	{
		if (p->weaponowned[i])
			p->readyweapon = p->pendingweapon = i;
	}

	if (!Marshmallow_GradedWeapons)
	{
		//StripWeapons(player);  // TODO: test that this does not mess something else up
		return;
	}

	if ( organic_levelchange )  // NEW so we don't get these weapons on normal level changes
		return;

//#if 0
	if ( gamemap == 1 )
	{
		StripWeapons(player);  // HACK: awful way to handle this...
		return;
	}
//#endif

	 if ( gamemap >= 2 )  // make defines for these level #'s
	 {
		p->weaponowned[wp_chainsaw] = true; 
		p->weaponowned[wp_shotgun] = true; 

		p->ammo[am_shell] += 4;

		p->readyweapon = wp_shotgun;
	 }

	 if ( gamemap >= 3 )  // make defines for these level #'s
	 {
		p->weaponowned[wp_chaingun] = true; 

		p->readyweapon = wp_chaingun;
	 }

	 if ( gamemap >= 5 )  // make defines for these level #'s
	 {
		p->weaponowned[wp_missile] = true; 

		p->ammo[am_misl] += 2;

		//p->readyweapon = wp_chaingun;
	 }

	 if (gamemode == commercial)  // DOOM II and Final DOOM
	 {
		 if ( gamemap >= 7 )
		 {
			p->weaponowned[wp_supershotgun] = true; 

			p->ammo[am_shell] += 4;		// intentionally adding to the previous +=4

			p->readyweapon = wp_supershotgun;
		 }

		 if ( gamemap >= 8 )
		 {
			p->weaponowned[wp_plasma] = true; 

			p->ammo[am_cell] += 20;

			p->readyweapon = wp_supershotgun;
		 }

		 if ( gamemap >= 8 )
		 {
			p->weaponowned[wp_bfg] = true; 

			p->ammo[am_cell] += 40;	

			p->readyweapon = wp_supershotgun;
		 }
	 }
	 else   // DOOM Retail and Shareware
	 {
		if ( gamemap >= 4 
			 && Marshmallow_Doom1SSG)		// for SSG in Doom1
		{
			p->weaponowned[wp_supershotgun] = true; 

			p->ammo[am_shell] += 4;		// intentionally adding to the previous +=4

			p->readyweapon = wp_supershotgun;
		}

	 	if ( gamemap >= 6 ) 
		{
			p->weaponowned[wp_plasma] = true; 

			p->ammo[am_cell] += 20;
		}

		if ( /*gameepisode > 2				// TODO: episode-specific  // e3m3?
			&&*/ gamemap >= 7 )
		{
			p->weaponowned[wp_bfg] = true; 

			p->ammo[am_cell] += 40;	
		}
	  }

	SaveArsenal(player);   // so that we respawn with these weapons
}


void HandleRespawnInventory(int player)
{
	int i;
	player_t* p = &players[player];

	if (Marshmallow_KeepKeys)
		RestoreKeyRing(player);  

	if (Marshmallow_KeepWeapons && !deathmatch)  // added !deathmatch March 2019
	{
		RestoreArsenal(player);
		
		for (i=0; i<=NUMWEAPONS; i++)  // Find best weapon we have and set as readyweapon
		{
			if (i == wp_bfg || i == wp_chainsaw)
				continue;

			if (p->weaponowned[i])
				p->readyweapon = p->pendingweapon = i;
		}
	}
	else  // Vanilla behavior
	{
		p->readyweapon = p->pendingweapon = wp_pistol; 
	}
}


void RestoreArsenal(int player)
{
	player_t* p;
	weapontype_t i;

	p = &players[player];

	for (i=0; i<NUMWEAPONS; i++) 
	{
		if (Marshmallow_Arsenal[player][i] == true)
		{
			P_GiveWeapon(p, i, false);

			switch (i)
			{
			case wp_shotgun:
			case wp_supershotgun:
				p->ammo[am_shell] = RESPAWN_SHELLS;  
				break;

			case wp_plasma:
			case wp_bfg:
				p->ammo[am_cell] = RESPAWN_CELLS;  
				break;
			
			case wp_missile:
				p->ammo[am_misl] = RESPAWN_ROCKETS;
				break;
			}
		}
	}
}