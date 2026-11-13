//
//  Copyright(C) 2017 Christoph Oelckers
//  Copyright(C) 2021 Roman Fomin
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.

#ifndef G_UMAPINFO_H
#define G_UMAPINFO_H

#include "d_mode.h"
#include "doomtype.h"

typedef enum
{
    MapInfo_LabelClear = (1u << 0),

    MapInfo_EndGameArt = (1u << 2),
    MapInfo_EndGameStandard = (1u << 3),
    MapInfo_EndGameCast = (1u << 4),
    MapInfo_EndGameBunny = (1u << 5),
    MapInfo_EndGame = (MapInfo_EndGameArt | MapInfo_EndGameStandard |
                       MapInfo_EndGameCast | MapInfo_EndGameBunny),
    MapInfo_EndGameClear = (1u << 6),

    MapInfo_NoIntermission = (1u << 7),
    MapInfo_InterTextClear = (1u << 8),
    MapInfo_InterTextSecretClear = (1u << 9),

    MapInfo_BossActionClear = (1u << 10)
} mapinfo_flags_t;

typedef struct
{
    int type;
    int special;
    int tag;
} bossaction_t;

typedef struct mapentry_s
{
    char *lumpname;
    char *levelname;
    char *label;
    char *intertext;
    char *intertextsecret;
    char *author;
    char levelpic[9];
    char nextmap[9];
    char nextsecret[9];
    char music[9];
    char skytexture[9];
    char endpic[9];
    char exitpic[9];
    char enterpic[9];
    char interbackdrop[9];
    char intermusic[9];
    int partime;
    bossaction_t *bossactions;
    mapinfo_flags_t flags;
} mapentry_t;

extern boolean mapinfo_episodes;
extern boolean mapinfo_partimes;
extern boolean mapinfo_mapxy;
extern boolean mapinfo_finale;

typedef void AddEpisodeFunc(const char *, const char *, const char *, char);
typedef void ClearEpisodeFunc(void);

void G_ParseMapInfo(int lumpnum, GameMission_t mission, GameMode_t mode,
                    AddEpisodeFunc add, ClearEpisodeFunc clear);

mapentry_t *G_LookupMapinfo(int episode, int map);

boolean G_ValidateMapName(const char *mapname, int *episode, int *map);

char *G_MapName(int e, int m);

#endif
