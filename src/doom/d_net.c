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
//	DOOM Network game communication and protocol,
//	all OS independend parts.
//

#include <stdlib.h>

#include "d_main.h"
#include "m_argv.h"
#include "m_menu.h"
#include "m_misc.h"
#include "i_system.h"
#include "i_timer.h"
#include "i_video.h"
#include "g_game.h"
#include "doomdef.h"
#include "doomstat.h"
#include "w_checksum.h"
#include "w_wad.h"

#include "deh_main.h"

#include "d_loop.h"

#include "marshmallow.h"  // [marshmallow]
marshmallow_netgame_flags MarshmallowSettings;

ticcmd_t *netcmds;

// Called when a player leaves the game

static void PlayerQuitGame(player_t *player)
{
    static char exitmsg[80];
    unsigned int player_num;

    player_num = player - players;

    // Do this the same way as Vanilla Doom does, to allow dehacked
    // replacements of this message

    M_StringCopy(exitmsg, DEH_String("Player 1 left the game"),
                 sizeof(exitmsg));

    exitmsg[7] += player_num;

    playeringame[player_num] = false;
    players[consoleplayer].message = exitmsg;
    // [crispy] don't interpolate players who left the game
    player->mo->interp = false;

    // TODO: check if it is sensible to do this:

    if (demorecording) 
    {
        G_CheckDemoStatus ();
    }
}

static void RunTic(ticcmd_t *cmds, boolean *ingame)
{
    extern boolean advancedemo;
    unsigned int i;

    // Check for player quits.

    for (i = 0; i < MAXPLAYERS; ++i)
    {
        if (bot_in_game[i]) 	// [marshmallow]
            continue;

        if (!demoplayback && playeringame[i] && !ingame[i])
        {
            PlayerQuitGame(&players[i]);
        }
    }

    netcmds = cmds;

    // check that there are players in the game.  if not, we cannot
    // run a tic.

    if (advancedemo)
        D_DoAdvanceDemo ();

    G_Ticker ();
}

static loop_interface_t doom_loop_interface = {
    D_ProcessEvents,
    G_BuildTiccmd,
    RunTic,
    M_Ticker
};


// Load game settings from the specified structure and
// set global variables.

static void LoadGameSettings(net_gamesettings_t *settings)
{
    unsigned int i;

    deathmatch = settings->deathmatch;
    startepisode = settings->episode;
    startmap = settings->map;
    startskill = settings->skill;
    startloadgame = settings->loadgame;
    lowres_turn = settings->lowres_turn;
    nomonsters = settings->nomonsters;
    fastparm = settings->fast_monsters;
    respawnparm = settings->respawn_monsters;
    timelimit = settings->timelimit;
    consoleplayer = settings->consoleplayer;

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

    if (!deathmatch)
    {
        Marshmallow_WeaponsStay = settings->MarshmallowSettings.WeaponsStay;
    }
    else
    {
        if ( deathmatch == 3 )
            Marshmallow_WeaponsStay = true;
    }

    Marshmallow_AlternateUltraViolence = settings->MarshmallowSettings.AlternateUltraViolence;
    Marshmallow_AlternateNightmare = settings->MarshmallowSettings.AlternateNightmare;
    Marshmallow_RespawnInNightmare = settings->MarshmallowSettings.RespawnInNightmare;

    if (Marshmallow_AlternateUltraViolence)  // any better place for this?
        Marshmallow_InitAltUltraViolence();

    if (Marshmallow_AlternateNightmare)  // any better place for this?
        Marshmallow_InitAltNightmare();

    Marshmallow_TrueInvisibility = settings->MarshmallowSettings.TrueInvisibility;

//    Marshmallow_PlayerCollision = settings->MarshmallowSettings.PlayerCollision;

    Marshmallow_Sandbox = settings->MarshmallowSettings.SandboxMode;

    if (Marshmallow_Sandbox)  // any better place for this?
        InitSandbox();

    Marshmallow_WitholdSSG = settings->MarshmallowSettings.Marshmallow_WitholdSSG;
    Marshmallow_Doom1SSG = settings->MarshmallowSettings.Marshmallow_Doom1SSG;
    SSG_Level = settings->MarshmallowSettings.SSG_Level;
    Doom1SSG_Level = settings->MarshmallowSettings.Doom1SSG_Level;

    physics_mode = settings->MarshmallowSettings.physics_mode;

    upgrade_chance = settings->MarshmallowSettings.upgrade_chance;
    MonsterHitpointsScale = settings->MarshmallowSettings.MonsterHitpointsScale;

    Marshmallow_TreasureMode = settings->MarshmallowSettings.TreasureMode;

    dm_timelimit = settings->MarshmallowSettings.dm_timelimit;
    dm_fraglimit = settings->MarshmallowSettings.dm_fraglimit;

    Marshmallow_RandomItems = settings->MarshmallowSettings.RandomItems;

    Marshmallow_GibMode = settings->MarshmallowSettings.gibmode;

    Marshmallow_AlternateLighting = settings->MarshmallowSettings.AlternateLighting;

    Marshmallow_DangerousBFG = settings->MarshmallowSettings.DangerousBFG;
    Marshmallow_BarrelPushing = settings->MarshmallowSettings.BarrelPushing;
    Marshmallow_DeathmatchWeapons = settings->MarshmallowSettings.DeathmatchWeapons;
    Marshmallow_KillOnExit = settings->MarshmallowSettings.DM_KillOnExit;
    Marshmallow_AllowExit = settings->MarshmallowSettings.DM_AllowExit;
    barrel_fx = settings->MarshmallowSettings.barrel_fx;
    Marshmallow_EpicBossDeaths = settings->MarshmallowSettings.EpicBossDeaths;
    Marshmallow_ResizeMonsters = settings->MarshmallowSettings.ResizeMonsters;

    crispy->fliplevels = settings->MarshmallowSettings.fliplevels;

    if (lowres_turn)
    {
        printf("NOTE: Turning resolution is reduced; this is probably "
               "because there is a client recording a Vanilla demo.\n");
    }

    for (i = 0; i < MAXPLAYERS; ++i)
    {
        playeringame[i] = i < settings->num_players;
    }
}

// Save the game settings from global variables to the specified
// game settings structure.

static void SaveGameSettings(net_gamesettings_t *settings)
{
    // Fill in game settings structure with appropriate parameters
    // for the new game

    settings->deathmatch = deathmatch;
    settings->episode = startepisode;
    settings->map = startmap;
    settings->skill = startskill;
    settings->loadgame = startloadgame;
    settings->gameversion = gameversion;
    settings->nomonsters = nomonsters;
    settings->fast_monsters = fastparm;
    settings->respawn_monsters = respawnparm;
    settings->timelimit = timelimit;

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

    if (!deathmatch)
    {
        settings->MarshmallowSettings.WeaponsStay = Marshmallow_WeaponsStay;
    }
    else
    {
        if ( deathmatch == 3 )
            settings->MarshmallowSettings.WeaponsStay = true;
    }

    settings->MarshmallowSettings.AlternateUltraViolence = Marshmallow_AlternateUltraViolence;
    settings->MarshmallowSettings.AlternateNightmare = Marshmallow_AlternateNightmare;
    settings->MarshmallowSettings.RespawnInNightmare = Marshmallow_RespawnInNightmare;

    settings->MarshmallowSettings.TrueInvisibility = Marshmallow_TrueInvisibility;

    //settings->MarshmallowSettings.PlayerCollision = Marshmallow_PlayerCollision;

    settings->MarshmallowSettings.SandboxMode = Marshmallow_Sandbox;

    settings->MarshmallowSettings.Marshmallow_WitholdSSG = Marshmallow_WitholdSSG;
    settings->MarshmallowSettings.Marshmallow_Doom1SSG = Marshmallow_Doom1SSG;
    settings->MarshmallowSettings.SSG_Level = SSG_Level;
    settings->MarshmallowSettings.Doom1SSG_Level = Doom1SSG_Level;

    settings->MarshmallowSettings.physics_mode = physics_mode;

    settings->MarshmallowSettings.upgrade_chance = upgrade_chance;
    settings->MarshmallowSettings.MonsterHitpointsScale = MonsterHitpointsScale;

    settings->MarshmallowSettings.TreasureMode = Marshmallow_TreasureMode;

    settings->MarshmallowSettings.dm_timelimit = dm_timelimit;
    settings->MarshmallowSettings.dm_fraglimit = dm_fraglimit;

    settings->MarshmallowSettings.RandomItems = Marshmallow_RandomItems;

    settings->MarshmallowSettings.gibmode = Marshmallow_GibMode;

    settings->MarshmallowSettings.AlternateLighting = Marshmallow_AlternateLighting;

    settings->MarshmallowSettings.ResizeMonsters = Marshmallow_ResizeMonsters;
    settings->MarshmallowSettings.EpicBossDeaths = Marshmallow_EpicBossDeaths;
    settings->MarshmallowSettings.barrel_fx = barrel_fx;
    settings->MarshmallowSettings.DM_AllowExit = Marshmallow_AllowExit;
    settings->MarshmallowSettings.DM_KillOnExit = Marshmallow_KillOnExit;
    settings->MarshmallowSettings.DeathmatchWeapons = Marshmallow_DeathmatchWeapons;
    settings->MarshmallowSettings.BarrelPushing = Marshmallow_BarrelPushing;
    settings->MarshmallowSettings.DangerousBFG = Marshmallow_DangerousBFG;

    settings->MarshmallowSettings.fliplevels = crispy->fliplevels;

    settings->lowres_turn = (M_ParmExists("-record")
                         && !M_ParmExists("-longtics"))
                          || M_ParmExists("-shorttics");
}

static void InitConnectData(net_connect_data_t *connect_data)
{
    boolean shorttics;

    connect_data->max_players = MAXPLAYERS;
    connect_data->drone = false;

    //!
    // @category net
    //
    // Run as the left screen in three screen mode.
    //

    if (M_CheckParm("-left") > 0)
    {
        viewangleoffset = ANG90;
        connect_data->drone = true;
    }

    //! 
    // @category net
    //
    // Run as the right screen in three screen mode.
    //

    if (M_CheckParm("-right") > 0)
    {
        viewangleoffset = ANG270;
        connect_data->drone = true;
    }

    //
    // Connect data
    //

    // Game type fields:

    connect_data->gamemode = gamemode;
    connect_data->gamemission = gamemission;

    //!
    // @category demo
    //
    // Play with low turning resolution to emulate demo recording.
    //

    shorttics = M_ParmExists("-shorttics");

    // Are we recording a demo? Possibly set lowres turn mode

    connect_data->lowres_turn = (M_ParmExists("-record")
                             && !M_ParmExists("-longtics"))
                              || shorttics;

    // Read checksums of our WAD directory and dehacked information

    W_Checksum(connect_data->wad_sha1sum);
    DEH_Checksum(connect_data->deh_sha1sum);

    // Are we playing with the Freedoom IWAD?

    connect_data->is_freedoom = W_CheckNumForName("FREEDOOM") >= 0;
}

void D_ConnectNetGame(void)
{
    net_connect_data_t connect_data;

    InitConnectData(&connect_data);
    realnetgame = netgame = D_InitNetGame(&connect_data);  // [marshmallow] Added realnetgame flag

    //!
    // @category net
    //
    // Start the game playing as though in a netgame with a single
    // player.  This can also be used to play back single player netgame
    // demos.
    //

    if (M_CheckParm("-solo-net") > 0)
    {
        netgame = true;
    }
}

//
// D_CheckNetGame
// Works out player numbers among the net participants
//
void D_CheckNetGame (void)
{
    net_gamesettings_t settings;

    if (netgame)
    {
        autostart = true;
    }

    D_RegisterLoopCallbacks(&doom_loop_interface);

    SaveGameSettings(&settings);
    D_StartNetGame(&settings, NULL);
    LoadGameSettings(&settings);

    DEH_printf("startskill %i  deathmatch: %i  startmap: %i  startepisode: %i\n",
               startskill, deathmatch, startmap, startepisode);

    DEH_printf("player %i of %i (%i nodes)\n",
               consoleplayer+1, settings.num_players, settings.num_players);

    // Show players here; the server might have specified a time limit

    if (timelimit > 0 && deathmatch)
    {
        // Gross hack to work like Vanilla:

        if (timelimit == 20 && M_CheckParm("-avg"))
        {
            DEH_printf("Austin Virtual Gaming: Levels will end "
                           "after 20 minutes\n");
        }
        else
        {
            DEH_printf("Levels will end after %d minute", timelimit);
            if (timelimit > 1)
                printf("s");
            printf(".\n");
        }
    }
}

