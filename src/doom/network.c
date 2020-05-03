#include "..\net_defs.h"
#include "..\net_packet.h"


void NET_WriteMarshmallowSettings(net_packet_t *packet, net_gamesettings_t *settings)
{
    NET_WriteInt8(packet, settings->MarshmallowSettings.FriendlyFire);
    NET_WriteInt8(packet, settings->MarshmallowSettings.MirrorDamage);
    NET_WriteInt8(packet, settings->MarshmallowSettings.SelfDamage);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DropGoodies);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DropBackpack);
    NET_WriteInt8(packet, settings->MarshmallowSettings.CoopItemRespawn);

    NET_WriteInt8(packet, settings->MarshmallowSettings.ConservePowerups);
	NET_WriteInt8(packet, settings->MarshmallowSettings.KeepWeapons);
    NET_WriteInt8(packet, settings->MarshmallowSettings.KeepKeys);
    NET_WriteInt8(packet, settings->MarshmallowSettings.SaveItems);

	NET_WriteInt8(packet, settings->MarshmallowSettings.GradedWeapons);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DeathmatchWeapons);
    NET_WriteInt8(packet, settings->MarshmallowSettings.WeaponsStay);

	NET_WriteInt8(packet, settings->MarshmallowSettings.AlternateUltraViolence);
	NET_WriteInt8(packet, settings->MarshmallowSettings.AlternateNightmare);
	NET_WriteInt8(packet, settings->MarshmallowSettings.RespawnInNightmare);

	NET_WriteInt8(packet, settings->MarshmallowSettings.TrueInvisibility);

	NET_WriteInt8(packet, settings->MarshmallowSettings.PlayerCollision);

	NET_WriteInt8(packet, settings->MarshmallowSettings.SandboxMode);

	NET_WriteInt8(packet, settings->MarshmallowSettings.Marshmallow_WitholdSSG);
	NET_WriteInt8(packet, settings->MarshmallowSettings.Marshmallow_Doom1SSG);
	NET_WriteInt8(packet, settings->MarshmallowSettings.SSG_Level);
	NET_WriteInt8(packet, settings->MarshmallowSettings.Doom1SSG_Level);

	NET_WriteInt8(packet, settings->MarshmallowSettings.physics_mode);

	NET_WriteInt8(packet, settings->MarshmallowSettings.upgrade_chance);
	NET_WriteInt8(packet, settings->MarshmallowSettings.MonsterHitpointsScale);

	NET_WriteInt8(packet, settings->MarshmallowSettings.TreasureMode);

	NET_WriteInt8(packet, settings->MarshmallowSettings.dm_timelimit);
	NET_WriteInt8(packet, settings->MarshmallowSettings.dm_fraglimit);

	NET_WriteInt8(packet, settings->MarshmallowSettings.RandomItems);

	NET_WriteInt8(packet, settings->MarshmallowSettings.gibmode);

	NET_WriteInt8(packet, settings->MarshmallowSettings.AlternateLighting);

    NET_WriteInt8(packet, settings->MarshmallowSettings.ResizeMonsters);
    NET_WriteInt8(packet, settings->MarshmallowSettings.EpicBossDeaths);
    NET_WriteInt8(packet, settings->MarshmallowSettings.barrel_fx);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DM_AllowExit);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DM_KillOnExit);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DeathmatchWeapons);
    NET_WriteInt8(packet, settings->MarshmallowSettings.BarrelPushing);
    NET_WriteInt8(packet, settings->MarshmallowSettings.DangerousBFG);

	NET_WriteInt8(packet, settings->MarshmallowSettings.fliplevels);
}


boolean NET_ReadMarshmallowSettings(net_packet_t *packet, net_gamesettings_t *settings)
{
    return 
	
	// Typical settings:

	NET_ReadInt8(packet, (unsigned int *) &settings->ticdup)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->extratics)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->deathmatch)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->nomonsters)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->fast_monsters)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->respawn_monsters)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->episode)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->map)
	&& NET_ReadSInt8(packet, &settings->skill)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->gameversion)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->lowres_turn)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->new_sync)
	&& NET_ReadInt32(packet, (unsigned int *) &settings->timelimit)
	&& NET_ReadSInt8(packet, (signed int *) &settings->loadgame)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->random)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->num_players)
	&& NET_ReadSInt8(packet, (signed int *) &settings->consoleplayer)

	// Marshmallow settings:
		
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.FriendlyFire)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.MirrorDamage)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.SelfDamage)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DropGoodies)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DropBackpack)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.CoopItemRespawn)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.ConservePowerups)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.KeepWeapons)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.KeepKeys)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.SaveItems)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.GradedWeapons)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DeathmatchWeapons)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.WeaponsStay)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.AlternateUltraViolence)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.AlternateNightmare)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.RespawnInNightmare)
	
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.TrueInvisibility)
		
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.PlayerCollision)
		
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.SandboxMode)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.Marshmallow_WitholdSSG)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.Marshmallow_Doom1SSG)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.SSG_Level)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.Doom1SSG_Level)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.physics_mode)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.upgrade_chance)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.MonsterHitpointsScale)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.TreasureMode)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.dm_timelimit)
	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.dm_fraglimit)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.RandomItems)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.gibmode)

	&& NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.AlternateLighting)

    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.ResizeMonsters)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.EpicBossDeaths)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.barrel_fx)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DM_AllowExit)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DM_KillOnExit)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DeathmatchWeapons)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.BarrelPushing)
    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.DangerousBFG)

    && NET_ReadInt8(packet, (unsigned int *) &settings->MarshmallowSettings.fliplevels);
}



#if 0
// These are unused as we were getting a stack overflow when using these functions
void SaveMarshmallowSettings(net_gamesettings_t *settings) 
{
	settings->MarshmallowSettings.FriendlyFire = Marshmallow_FriendlyFire;
	settings->MarshmallowSettings.MirrorDamage = Marshmallow_MirrorDamage;
	settings->MarshmallowSettings.SelfDamage = Marshmallow_SelfDamage;
	settings->MarshmallowSettings.DropGoodies = Marshmallow_DropGoodies;
	settings->MarshmallowSettings.DropBackpack = Marshmallow_DropBackpack;
	settings->MarshmallowSettings.CoopItemRespawn = Marshmallow_CoopItemRespawn;

	settings->MarshmallowSettings.ConservePowerups = Marshmallow_ConservePowerups;
	settings->MarshmallowSettings.KeepWeapons = Marshmallow_KeepWeapons;
	settings->MarshmallowSettings.KeepKeys = Marshmallow_KeepKeys;
	settings->MarshmallowSettings.SaveItems = Marshmallow_SaveItems;

	settings->MarshmallowSettings.GradedWeapons = Marshmallow_GradedWeapons;
	settings->MarshmallowSettings.DeathmatchWeapons = Marshmallow_DeathmatchWeapons;
	settings->MarshmallowSettings.WeaponsStay = Marshmallow_WeaponsStay;

	settings->MarshmallowSettings.RespawnInNightmare = Marshmallow_RespawnInNightmare;

	settings->MarshmallowSettings.TrueInvisibility = Marshmallow_TrueInvisibility;

	settings->MarshmallowSettings.PlayerCollision = Marshmallow_PlayerCollision;

	settings->MarshmallowSettings.SandboxMode = Marshmallow_Sandbox;

	settings->MarshmallowSettings.Marshmallow_WitholdSSG = Marshmallow_WitholdSSG;
	settings->MarshmallowSettings.Marshmallow_Doom1SSG = Marshmallow_Doom1SSG;
	settings->MarshmallowSettings.SSG_Level = SSG_Level;
	settings->MarshmallowSettings.Doom1SSG_Level = Doom1SSG_Level;

	settings->MarshmallowSettings.physics_mode = physics_mode;

	settings->MarshmallowSettings.MonsterHitpointsScale = MonsterHitpointsScale;

	settings->MarshmallowSettings.TreasureMode = Marshmallow_TreasureMode;

	settings->MarshmallowSettings.dm_timelimit = dm_timelimit;
	settings->MarshmallowSettings.dm_fraglimit = dm_fraglimit; 
}


void LoadMarshmallowSettings(net_gamesettings_t *settings)
{
	Marshmallow_FriendlyFire = settings->MarshmallowSettings.FriendlyFire;
	Marshmallow_MirrorDamage = settings->MarshmallowSettings.MirrorDamage;
	Marshmallow_SelfDamage = settings->MarshmallowSettings.SelfDamage;
	Marshmallow_DropGoodies = settings->MarshmallowSettings.DropGoodies;
	Marshmallow_DropBackpack = settings->MarshmallowSettings.DropBackpack;
	Marshmallow_CoopItemRespawn = settings->MarshmallowSettings.CoopItemRespawn;

	Marshmallow_ConservePowerups = settings->MarshmallowSettings.ConservePowerups;
	Marshmallow_KeepWeapons = settings->MarshmallowSettings.KeepWeapons;
	Marshmallow_KeepKeys = settings->MarshmallowSettings.KeepKeys;
	Marshmallow_SaveItems = settings->MarshmallowSettings.SaveItems;

	Marshmallow_GradedWeapons = settings->MarshmallowSettings.GradedWeapons;
	Marshmallow_DeathmatchWeapons = settings->MarshmallowSettings.DeathmatchWeapons;
	Marshmallow_WeaponsStay = settings->MarshmallowSettings.WeaponsStay;

	Marshmallow_RespawnInNightmare = settings->MarshmallowSettings.RespawnInNightmare;

	Marshmallow_TrueInvisibility = settings->MarshmallowSettings.TrueInvisibility;

	Marshmallow_PlayerCollision = settings->MarshmallowSettings.PlayerCollision;

	Marshmallow_Sandbox = settings->MarshmallowSettings.SandboxMode;

	Marshmallow_WitholdSSG = settings->MarshmallowSettings.Marshmallow_WitholdSSG;
	Marshmallow_Doom1SSG = settings->MarshmallowSettings.Marshmallow_Doom1SSG;
	SSG_Level = settings->MarshmallowSettings.SSG_Level;
	Doom1SSG_Level = settings->MarshmallowSettings.Doom1SSG_Level;

	physics_mode = settings->MarshmallowSettings.physics_mode;

	MonsterHitpointsScale = settings->MarshmallowSettings.MonsterHitpointsScale;

	Marshmallow_TreasureMode = settings->MarshmallowSettings.TreasureMode;

	dm_timelimit = settings->MarshmallowSettings.dm_timelimit;
	dm_fraglimit = settings->MarshmallowSettings.dm_fraglimit; 
}

#endif