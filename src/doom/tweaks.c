////////////////////////////////////////////////////////
//
//  TWEAKS.C:  Gameplay tweaks to weapons, enemies, etc
//
////////////////////////////////////////////////////////

#include "marshmallow.h"
	

//
// MONSTERS
//

int UpgradeMonsters(int i)
{
	boolean doom2;
	int monster = NULL; 

	if (!upgrade_chance)
		return i;

	if ( GetGameType() == DOOM1 )
		doom2 = false; 
	else
		doom2 = true;

	switch (i)
	{
	case MT_POSSESSED:

		if ( PercentChance( upgrade_chance ) )
			monster = MT_SHOTGUY;
		else
			return i;

		break;

	case MT_SHOTGUY:

		if ( PercentChance( upgrade_chance ) )
		{
			if (doom2)
			{
				if ( PercentChance(50) )
					monster = MT_CHAINGUY;  
				else
					monster = MT_TROOP;
			}
			else
				monster = MT_TROOP;
		}
		else
		{
			return i;
		}
		
		break;

	case MT_TROOP:
		
		if ( PercentChance( upgrade_chance ) )
		{
			if (doom2)
			{
				if ( PercentChance(50) )
				{
					if ( PercentChance(50) )
						monster = MT_PAIN;  
					else
						monster = MT_HEAD;
				}
				else
					monster = MT_UNDEAD;  // was MT_FATSO but he's too fat
			}
			else
			{
				if ( PercentChance(50) )
				{
					monster = MT_HEAD;
				}
				else
				{
					if ( PercentChance(50) )
						monster = MT_SHADOWS;
					else
						monster = MT_SERGEANT;
				}
			}
		}
		else
		{
			return i;
		}

		break;

	case MT_CHAINGUY:

		if ( PercentChance( upgrade_chance ) )
			monster = MT_KNIGHT;
		else
			return i;

		break;

	case MT_SERGEANT:
	case MT_SHADOWS:

		if ( PercentChance( upgrade_chance ) )
		{
			if (doom2)
			{
				if ( PercentChance(50) )
					monster = MT_UNDEAD; 
				else
					monster = MT_HEAD;
			}
			else
			{
				if ( PercentChance(27) )   // yeah, that's right... 27
					monster = MT_HEAD;
				else
					return i;
			}
		}
		else
		{
			return i;
		}

		break;

	case MT_HEAD:

		if (PLAYING_DOOM1)
		{
			if (E3M8)   // when upgraded, E3M8 now features the bosses from all three episodes
			{
				if ( !e3m8_caco_upgrade )  //  first pass: cyberdemon
				{
					e3m8_caco_upgrade = true;

					return MT_CYBORG;
				}
				else
				{
					return MT_BRUISER;  // second pass: baron
				}
			}
		}
		
		if (doom2)
		{
			if ( PercentChance(50) )
				monster = MT_PAIN;
			else
				return i;
		}

		break;

	case MT_UNDEAD:

		if ( PercentChance( upgrade_chance ) )
			monster = MT_KNIGHT;
		else
			return i;

		break;

	case MT_BABY:

		if (MAP07)  // leave MAP07 alone
			return i;		
		
		if ( PercentChance( upgrade_chance ) )
			monster = MT_FATSO;
		else
			return i;

		break;

	case MT_KNIGHT:

		if ( PercentChance( upgrade_chance ) )
			monster = MT_BRUISER;
		else
			return i;

		break;

	case MT_FATSO:

		if (MAP07)  // leave MAP07 alone
			return i;

		if ( PercentChance( upgrade_chance ) )
			monster = MT_BRUISER;
		else
			return i;

		break;

	case MT_VILE:

		if ( PercentChance( upgrade_chance ) )
			monster = MT_BRUISER;
		else
			return i;

		break;

	case MT_BRUISER:

		if (PLAYING_DOOM1)
		{
			if (E1M8)  // leave E1M8 bosses alone
				return i;

			if (E3M8)  // don't upgrade the baron in E3M8
				return i;
		}

		if (PLAYING_DOOM2)
		{
			return i;  // don't upgrade any barons in Doom II
		}

		if ( PercentChance( upgrade_chance ) )
			monster = MT_CYBORG;
		else
			return i;

		break;

	case MT_WOLFSS:

		if ( PercentChance( upgrade_chance ) )
			monster = GetRandomDoom2Monster(i);
		else
			return i;

		break;  // NEW added 8-21-19

	case MT_SKULL:

		// Special case for spicing-up E2M8...

		if (PLAYING_DOOM1 && E2M8)
		{
			monster = GetRandomDoom1Monster(i);

			// TODO:  Doom2 monsters too if we are wad stealing
		}

		break;

	default:
		return i;  // return it now so we don't change height/radius
	}

	if (!monster)
		return i;

	if (!Marshmallow_ResizeMonsters)
		return monster;

	switch (monster) 
	{
	case MT_HEAD:
		mobjinfo[MT_HEAD].radius = 20*FRACUNIT;   
		break;
	case MT_PAIN:
		mobjinfo[MT_PAIN].radius = 20*FRACUNIT; 
		break;
	case MT_FATSO:
		mobjinfo[MT_FATSO].radius = 20*FRACUNIT; 
		break;
	case MT_SERGEANT:
		mobjinfo[MT_SERGEANT].radius = 20*FRACUNIT; 
		break;
	case MT_SHADOWS:
		mobjinfo[MT_SHADOWS].radius = 20*FRACUNIT; 
		break;

	case MT_KNIGHT:
		mobjinfo[MT_KNIGHT].height = 56*FRACUNIT; 
		mobjinfo[MT_KNIGHT].radius = 20*FRACUNIT;
		break;
	case MT_BRUISER:
		mobjinfo[MT_BRUISER].height = 56*FRACUNIT; 
		mobjinfo[MT_BRUISER].radius = 30*FRACUNIT; 
		break;

	case MT_CYBORG:
		mobjinfo[MT_CYBORG].height = 90*FRACUNIT; 
		break;
	}

	return monster;
}

// TODO: move into header
#define VILECHANCE 88
#define BRUISERCHANCE 75
#define KNIGHTCHANCE 50
#define UNDEADCHANCE 25
#define CHAINGUYCHANCE 10


int GetRandomDoom2Monster(int i)
{
	int chance = GetRandomIntegerInRange(1, 100);

	if (chance > VILECHANCE)
		return MT_VILE;
	else if (chance > BRUISERCHANCE)
		return MT_BRUISER;
	else if (chance > KNIGHTCHANCE)
		return MT_KNIGHT;
	else if (chance > UNDEADCHANCE)
		return MT_UNDEAD;
	else if (chance > CHAINGUYCHANCE)
		return MT_CHAINGUY;
	else 
		return MT_SHOTGUY;
}

// move into header
//#define BRUISERCHANCE 88
#define CACOCHANCE 75
#define SPECTRECHANCE 50
#define DEMONCHANCE 25
#define IMPCHANCE 10


int GetRandomDoom1Monster(int i)
{
	int chance = GetRandomIntegerInRange(1, 100);

	if (chance > BRUISERCHANCE)
		return MT_BRUISER;
	else if (chance > CACOCHANCE)
		return MT_HEAD;
	else if (chance > SPECTRECHANCE)
		return MT_SHADOWS;
	else if (chance > DEMONCHANCE)
		return MT_SERGEANT;
	else if (chance > IMPCHANCE)
		return MT_TROOP;
	else 
		return MT_SHOTGUY;
}


void ChangePainDeath(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_AltPainDeath = true; 
	}
	else
	{
		Marshmallow_AltPainDeath = false;
	}
}


void NerfHP_LostSoul(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_NerfHP_LostSoul = true;
		mobjinfo[MT_SKULL].spawnhealth = NERFED_LOSTSOUL_SPAWNHEALTH;  
	}
	else
	{
		Marshmallow_NerfHP_LostSoul = false;
		mobjinfo[MT_SKULL].spawnhealth = 100;  
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}


void NerfHP_Cacodemon(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_NerfHP_Cacodemon = true;
		mobjinfo[MT_HEAD].spawnhealth = NERFED_CACO_SPAWNHEALTH;
	}
	else
	{
		Marshmallow_NerfHP_Cacodemon = false;
		mobjinfo[MT_HEAD].spawnhealth = 400;
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}


void NerfHP_Demons(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_NerfHP_Demons = true;
		mobjinfo[MT_SHADOWS].spawnhealth = mobjinfo[MT_SERGEANT].spawnhealth = 100;  
	}
	else
	{
		Marshmallow_NerfHP_Demons = false;
		mobjinfo[MT_SHADOWS].spawnhealth = mobjinfo[MT_SERGEANT].spawnhealth = 150;
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}


void NerfPC_Archvile(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_NerfPC_Archvile = true;
		mobjinfo[MT_VILE].painchance = 60;  // 100 seemed too easy
	}
	else
	{
		Marshmallow_NerfPC_Archvile = false;
		mobjinfo[MT_VILE].painchance = 10;  // VANILLA
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}


void NerfSpeed_Undead(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_NerfSpeed_Undead = true;
		mobjinfo[MT_UNDEAD].speed = 5;
		mobjinfo[MT_UNDEAD].reactiontime = 12; // vanilla is 8   (untested)
	}
	else
	{
		Marshmallow_NerfSpeed_Undead = false;
		mobjinfo[MT_UNDEAD].speed = 10;
		mobjinfo[MT_UNDEAD].reactiontime = 8;
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}


void NerfSkelMissile(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_NerfSkelMissile = true;
		mobjinfo[MT_TRACER].speed = NERFED_SKELSHOT_SPEED; 
	}
	else
	{
		Marshmallow_NerfSkelMissile = false;
		mobjinfo[MT_TRACER].speed = 10*FRACUNIT;
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}

boolean Marshmallow_BoostHP_Spiderboss; // move

void NerfHP_BoostHP_Spiderboss(boolean option_on)  // not used yet
{
	if (option_on)
	{
		Marshmallow_BoostHP_Spiderboss = true;
		mobjinfo[MT_SPIDER].spawnhealth = 4000;  // vanilla is 3000
	}
	else
	{
		Marshmallow_BoostHP_Spiderboss = false;
		mobjinfo[MT_SPIDER].spawnhealth = 3000;
	}
}

/*
void BoostHP_Chainguy(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_BoostHP_Chainguy = true;
		mobjinfo[MT_CHAINGUY].spawnhealth *= 1.5;  // NEW!
	}
	else
	{
		Marshmallow_BoostHP_Chainguy = false;
		mobjinfo[MT_CHAINGUY].spawnhealth = 70;
	}

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}
*/

void FixChainguySound(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_FixChainguySound = true;
		chaingunguy_attack_sound = sfx_chgun;
	}
	else
	{
		Marshmallow_FixChainguySound = false;
		chaingunguy_attack_sound = sfx_shotgn;	
	}
}


//
// WEAPONS/ITEMS
//

#define NUM_WEAPONS 6
mobjtype_t RandomWeapons[NUM_WEAPONS] = {  
	
	MT_SHOTGUN,
	MT_SUPERSHOTGUN,
	MT_CHAINGUN,
	MT_RPG,
	MT_PLASMA_RIFLE,
	MT_BFG9000,
	//MT_CHAINSAW

};

#define NUM_AMMO 4
mobjtype_t RandomAmmo[NUM_AMMO] = {

	MT_BOX_OF_BULLETS,
	MT_BOX_OF_SHELLS,
	MT_ROCKETAMMO,  
	MT_CELL,		
	
	// TODO: large ammo boxes here
};  

mobjtype_t RandomMedkit[2] = {

	//MT_MISC11,  // large medkit
	MT_MISC10,  // small medkit
	MT_HEALTH_BONUS,
};

mobjtype_t RandomArmor[2] = {

	MT_MISC0,  // green armor
	//MT_MISC1,  // megaarmor (blue)
	MT_ARMOR_BONUS,
};

mobjtype_t RandomPowerup[3] = {

	MT_INVISIBILITY, 
	MT_BERSERK,
	//MT_SOULSPHERE, 
};


int RandomizeItem(int i)
{
	// Full random mode, agnostic of input item type:
	if ( Marshmallow_RandomItems == 2 )  
	{
		switch (i)
		{
		case MT_CLIP:
		case MT_SHELLS:
		case MT_ROCKETAMMO:
		case MT_CELL:	
		case MT_MISC17:
		case MT_MISC23:
		case MT_MISC19:
		case MT_MISC21:
		//case MT_CHAINSAW:
		case MT_SHOTGUN:
		case MT_SUPERSHOTGUN:
		case MT_CHAINGUN:
		case MT_RPG:
		case MT_PLASMA_RIFLE:
		case MT_BFG9000:
		case MT_HEALTH_BONUS:    
		case MT_ARMOR_BONUS:
		case MT_MISC11:  // large medkit
		case MT_MISC10:  // stimpack
		case MT_GREENARMOR:   
		case MT_BLUEARMOR:   
			return GenerateRandomBonusItem();

		case MT_SOULSPHERE:
		case MT_INVISIBILITY:
		case MT_BERSERK:
			return GetRandomBonusItem(POWERUP);

		default:
			return i;  
		}
	}

	// Normal random items mode, items are replaced based on type:
	switch (i)
	{
	case MT_CLIP:
	case MT_SHELLS:
	case MT_ROCKETAMMO:
	case MT_CELL:
	case MT_MISC17: // box o' bullets
	case MT_MISC23: // box o' shells
	case MT_MISC19: // box o' rockets
	case MT_MISC21: // box o' cells
				
		i = GetRandomBonusItem(AMMO);
		return i;

	//case MT_CHAINSAW:
	case MT_SHOTGUN:
	case MT_SUPERSHOTGUN:
	case MT_CHAINGUN:
	case MT_RPG:
	case MT_PLASMA_RIFLE:
	case MT_BFG9000:

		i = GetRandomBonusItem(WEAPON);
		return i;

	case MT_HEALTH_BONUS:    
	case MT_ARMOR_BONUS:

		i = GenerateRandomBonusItem();
		return i;

	// In normal random items mode, medkits and armor are not replaced

	default:
		return i;
	}
}


mobjtype_t GenerateRandomBonusItem() 
{
	int r;

	r = GetRandomIntegerInRange(0, 99);

	if ( r < 25 )
		return GetRandomBonusItem(WEAPON);  // 25% chance of a weapon
	if ( r < 50 )
		return GetRandomBonusItem(AMMO);    // 25% chance of ammo
	if ( r < 72 )
		return GetRandomBonusItem(MEDKIT);  // 22% chance of a medkit
	if ( r < 94 )
		return GetRandomBonusItem(ARMOR);   // 22% chance of armor
	if ( r < 100 )
		return GetRandomBonusItem(POWERUP); // 6% chance of a powerup
}


mobjtype_t GetRandomBonusItem(int category)  
{
	int weapon, ammo, medkit, armor, powerup;

	switch (category)
	{
	case WEAPON:

		weapon = GetRandomIntegerInRange(0, NUM_WEAPONS-1);

		if ( GetGameType() == DOOM1 )
		{
			if ( RandomWeapons[weapon] == MT_SUPERSHOTGUN )
				weapon++;  // replace SSG with chaingun in Doom1
		}
	
		return RandomWeapons[weapon];

	case AMMO:

		ammo = GetRandomIntegerInRange(0, NUM_AMMO-1);
	
		return RandomAmmo[ammo];

	case MEDKIT:

		medkit = GetRandomIntegerInRange(0, 1);
	
		return RandomMedkit[medkit];

	case ARMOR:

		armor = GetRandomIntegerInRange(0, 1);
	
		return RandomArmor[armor];

	case POWERUP:

		powerup = GetRandomIntegerInRange(0, 1);
	
		return RandomPowerup[powerup];
	}
}


void BoostChainsaw(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_BoostedChainsaw = true;
		chainsaw_damage_multiplier = BOOSTED_CHAINSAW_DAMAGE;
	}
	else
	{
		Marshmallow_BoostedChainsaw = false;
		chainsaw_damage_multiplier = VANILLA_CHAINSAW_DAMAGE;			
	}
}

void BoostFist(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_BoostedFist = true;		
		fist_damage_multiplier = BOOSTED_FIST_DAMAGE;
	}
	else
	{
		Marshmallow_BoostedFist = false;
		fist_damage_multiplier = VANILLA_FIST_DAMAGE;
	}
}

void BoostBullets(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_BoostedBullets = true;
		gunshot_damage = 6;
	}
	else
	{
		Marshmallow_BoostedBullets = false;
		gunshot_damage = 5;
	}
}

void InfinitePistol(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_InfinitePistol = true;
		weaponinfo[wp_pistol].ammo = am_noammo;
	}
	else
	{
		Marshmallow_InfinitePistol = false;
		weaponinfo[wp_pistol].ammo = am_clip;
	}
}

void BalanceShotguns(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_BalanceShotguns = true;
		shotgun_pellets = EXTRA_PELLETS;
		ssg_range = MISSILERANGE / SSG_RANGE_NERF;

		//Marshmallow_BoostedBullets = true;
	}
	else
	{
		Marshmallow_BalanceShotguns = false;
		shotgun_pellets = VANILLA_PELLETS;
		ssg_range = MISSILERANGE;

		//Marshmallow_BoostedBullets = false;
	}
}

void BerserkAutoSwitch(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_BerserkAutoSwitch = true;
	}
	else
	{
		Marshmallow_BerserkAutoSwitch = false;
	}
}

void ChangeInvis(boolean option_on)
{
	if (option_on)
	{
		Marshmallow_ShadowInvis = true;
		mobjinfo[MT_INS].flags |= MF_SHADOW;  // Make the invis powerup invisible as to distinguish from our new health bonus item, the "Demonsphere"
	}
	else
	{
		Marshmallow_ShadowInvis = false;
		mobjinfo[MT_INS].flags ^= MF_SHADOW;  
	}
}

void ToggleGoodiesMode()
{
	Marshmallow_DropGoodies = !Marshmallow_DropGoodies;

	if (Marshmallow_DropGoodies)
		ChangeInvis(ON);
	else
		ChangeInvis(OFF);

	SHOW_MESSAGE DEH_String(CHANGEFFECT);
}

void Marshmallow_InitAltNightmare()
{	
	startskill = sk_nightmare;        
	gameskill = sk_nightmare;
	Marshmallow_RespawnInNightmare = false; 
	Marshmallow_AlternateNightmare = true;
	Marshmallow_AlternateUltraViolence = false;  // added 3-9-19

	upgrade_chance = NM2_UPGRADECHANCE; 
	
	//MonsterHitpointsScale = NM2_MONSTERSCALE;
}

void Marshmallow_InitAltUltraViolence()
{
	startskill = sk_hard;
	gameskill = sk_hard;

	upgrade_chance = UV2_UPGRADECHANCE;

	Marshmallow_AlternateUltraViolence = true;
}

void SetOptionsFromCfg()
{
	if (Marshmallow_BoostedChainsaw)
		BoostChainsaw(ON);

	if (Marshmallow_BoostedFist)
		BoostFist(ON);

	if (Marshmallow_BoostedBullets)
		BoostBullets(ON);

	if (Marshmallow_InfinitePistol)
		InfinitePistol(ON);

	if (Marshmallow_BalanceShotguns)
		BalanceShotguns(ON);

	if (!Marshmallow_NerfHP_LostSoul)
		NerfHP_LostSoul(OFF);
	
	if (Marshmallow_NerfHP_Cacodemon)
		NerfHP_Cacodemon(ON);

	if (Marshmallow_NerfHP_Demons)
		NerfHP_Demons(ON);
	
	if (!Marshmallow_NerfSpeed_Undead)
		NerfSpeed_Undead(OFF);

	if (!Marshmallow_NerfSkelMissile)
		NerfSkelMissile(OFF);

	if (!Marshmallow_AltPainDeath)
		ChangePainDeath(OFF);

	if (!Marshmallow_NerfPC_Archvile)
		NerfPC_Archvile(OFF);	

	if (!Marshmallow_FixChainguySound)
		FixChainguySound(OFF);

	//if (Marshmallow_AlternateLighting)
	//	Marshmallow_LightLevel = CRT_LIGHTLEVEL;
}

#if 0
void SetSpecialSkills()
{
	if (skill_selection == 5)
		Marshmallow_AlternateUltraViolence = true;
	else
		Marshmallow_AlternateUltraViolence = false;

	if (skill_selection == 6)   
	{
		Marshmallow_AlternateNightmare = true;
		Marshmallow_RespawnInNightmare = true;
	}
	else
	{
		Marshmallow_AlternateNightmare = false;
		Marshmallow_RespawnInNightmare = false;
	}
}
#endif