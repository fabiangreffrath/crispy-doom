//
// These functions allow us to share all game options with clients when starting a network game
//

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