//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2013-2017 Brad Harding
// Copyright(C) 2017-2022 Fabian Greffrath
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
//	Brightmaps for walls, floors, sprites and weapon states
//	Adapted from doomretro/src/r_data.c:97-209
//

#include "crispy.h"
#include "doomdef.h"
#include "r_bmaps.h"

// [crispy] brightmap data

static byte nobrightmap[256] = {0};

static byte fullbright[256] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

static byte surfaces[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
};


static byte consumables[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static byte hellstaff_world[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static byte flame[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,
};

static byte ethereal[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static byte energy[256] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

byte *dc_brightmap = nobrightmap;

// [crispy] brightmaps for textures

typedef struct
{
    const char *const texture;
    byte *colormask;
} fullbright_t;

static const fullbright_t fullbright_walls[] = {
    // [crispy] common textures
    {"DOOREXIT", surfaces},
    {"GRSKULL3", surfaces},
    {"SW1ON",    surfaces},
    {"SW1OFF",   surfaces},
    {"SW2ON",    surfaces},
    {"SW2OFF",   surfaces},
};

byte *R_BrightmapForTexName (const char *texname)
{
    int i;

    for (i = 0; i < arrlen(fullbright_walls); i++)
    {
        const fullbright_t *fullbright = &fullbright_walls[i];

        if (!strncasecmp(fullbright->texture, texname, 8))
        {
            return fullbright->colormask;
        }
    }

    return nobrightmap;
}

// [crispy] brightmaps for sprites

byte *R_BrightmapForSprite (const int type)
{
    if (crispy->brightmaps & BRIGHTMAPS_SPRITES)
    {
        switch (type)
        {
            // Enchanted Shield
            case SPR_SHD2:
            // Morph Ovum
            case SPR_EGGC:
            // Ring of Invincibility
            case SPR_INVU:
            // Chaos Device
            case SPR_ATLP:
            // Ethereal Crossbow
            case SPR_WBOW:
            // Phoenix Rod
            case SPR_WPHX:
            // Iron Lich
            case SPR_HEAD:
            {
                return consumables;
                break;
            }
            // Hell Staff
            case SPR_WSKL:
            {
                return hellstaff_world;
                break;
            }
            // Chandeiler
            case SPR_CHDL:
            // Fire Brazier
            case SPR_KFR1:
            // Serpent Torch
            case SPR_SRTC:
            // Torch (artifact)
            case SPR_TRCH:
            // Volcano
            case SPR_VLCO:
            // Sabreclaw
            case SPR_CLNK:
            {
                return flame;
                break;
            }
            // D'Sparil
            case SPR_SRCR:
            case SPR_SOR2:
            case SPR_SDTH:
            {
                return energy;
                break;
            }

            /* [JN] TODO
            // Wand Crystal
            case SPR_AMG1:
            // Crystal Geode
            case SPR_AMG2:
            // Ethereal Arrows
            case SPR_AMC1:
            // Claw Orb
            case SPR_AMB1:
            // Energy Orb
            case SPR_AMB2:
            // Lesser Runes:
            case SPR_AMS1:
            // Greater Runes:
            case SPR_AMS2:
            {
                return fullbright;
                break;
            }
            // Quiver of Ethereal Arrows
            case SPR_AMC2:
            {
                return consumables;
                break;
            }
            // Flame Orb
            case SPR_AMP1:
            // Infernal Orb
            case SPR_AMP2:
            {
                return flame;
                break;
            }
            // Disciple of D'Sparil
            case SPR_WZRD:
            {
                return energy;
                break;
            }
            */
        }
    }
    else
    {
        switch (type)
        {
            // Fire Brazier
            case SPR_KFR1:
            // Torch (artifact)
            case SPR_TRCH:
            {
                return fullbright;
                break;
            }
        }
    }

    return nobrightmap;
}

// [crispy] brightmaps for flats

static int bmapflatnum[12];

byte *R_BrightmapForFlatNum (const int num)
{
    if (crispy->brightmaps & BRIGHTMAPS_TEXTURES)
    {
        if (num == bmapflatnum[0]
        ||  num == bmapflatnum[1]
        ||  num == bmapflatnum[2]
        ||  num == bmapflatnum[3]
        ||  num == bmapflatnum[4])
        {
            return surfaces;
        }
    }

    return nobrightmap;
}

// [crispy] brightmaps for states

byte *R_BrightmapForState (const int state)
{

    if (crispy->brightmaps & BRIGHTMAPS_SPRITES)
    {
        switch (state)
        {
            // Gauntlets of the Necromancer
            case S_GAUNTLETATK1_3:
            case S_GAUNTLETATK1_4:
            case S_GAUNTLETATK1_5:
            case S_GAUNTLETATK1_6:
            case S_GAUNTLETATK1_7:
            {
                return consumables;
                break;
            }
            // Gauntlets of the Necromancer (powered)
            case S_GAUNTLETREADY2_1:
            case S_GAUNTLETREADY2_2:
            case S_GAUNTLETREADY2_3:
            case S_GAUNTLETATK2_1:
            case S_GAUNTLETATK2_2:
            case S_GAUNTLETATK2_3:
            case S_GAUNTLETATK2_4:
            case S_GAUNTLETATK2_5:
            case S_GAUNTLETATK2_6:
            case S_GAUNTLETATK2_7:
            {
                return flame;
                break;
            }
            // Elven Wand
            case S_GOLDWANDATK1_1:
            case S_GOLDWANDATK1_2:
            case S_GOLDWANDATK1_3:
            case S_GOLDWANDATK1_4:
            case S_GOLDWANDATK2_1:
            case S_GOLDWANDATK2_2:
            case S_GOLDWANDATK2_3:
            case S_GOLDWANDATK2_4:
            {
                return flame;
                break;
            }
            // Ethereal Crossbow
            case S_CRBOW1:
            case S_CRBOW2:
            case S_CRBOW3:
            case S_CRBOW4:
            case S_CRBOW5:
            case S_CRBOW6:
            case S_CRBOW7:
            case S_CRBOW8:
            case S_CRBOW9:
            case S_CRBOW10:
            case S_CRBOW11:
            case S_CRBOW12:
            case S_CRBOW13:
            case S_CRBOW14:
            case S_CRBOW15:
            case S_CRBOW16:
            case S_CRBOW17:
            case S_CRBOW18:
            case S_CRBOWDOWN:
            case S_CRBOWUP:
            case S_CRBOWATK1_1:
            case S_CRBOWATK1_2:
            case S_CRBOWATK1_3:
            case S_CRBOWATK1_4:
            case S_CRBOWATK1_5:
            case S_CRBOWATK1_6:
            case S_CRBOWATK1_7:
            case S_CRBOWATK1_8:
            case S_CRBOWATK2_1:
            case S_CRBOWATK2_2:
            case S_CRBOWATK2_3:
            case S_CRBOWATK2_4:
            case S_CRBOWATK2_5:
            case S_CRBOWATK2_6:
            case S_CRBOWATK2_7:
            case S_CRBOWATK2_8:
            {
                return ethereal;
                break;
            }
            // Dragon Claw
            case S_BLASTERATK1_1:
            case S_BLASTERATK1_2:
            case S_BLASTERATK1_3:
            case S_BLASTERATK1_4:
            case S_BLASTERATK1_5:
            case S_BLASTERATK1_6:
            case S_BLASTERATK2_1:
            case S_BLASTERATK2_2:
            case S_BLASTERATK2_3:
            case S_BLASTERATK2_4:
            case S_BLASTERATK2_5:
            case S_BLASTERATK2_6:
            {
                return energy;
                break;
            }
            // Hell Staff:
            // case S_HORNRODATK1_1:
            // case S_HORNRODATK1_2:
            // case S_HORNRODATK1_3:
            // case S_HORNRODATK2_1:
            // case S_HORNRODATK2_2:
            // case S_HORNRODATK2_3:
            // case S_HORNRODATK2_4:
            // case S_HORNRODATK2_5:
            // case S_HORNRODATK2_6:
            // case S_HORNRODATK2_7:
            // case S_HORNRODATK2_8:
            // case S_HORNRODATK2_9:
            // {
            //     return consumables;
            //     break;
            // }
            // Phoenix Rod (idle)
            case S_PHOENIXREADY:
            {
                return consumables;
                break;
            }
            // Phoenix Rod (attack)
            case S_PHOENIXATK1_1:
            case S_PHOENIXATK1_2:
            case S_PHOENIXATK1_3:
            case S_PHOENIXATK1_4:
            case S_PHOENIXATK1_5:
            case S_PHOENIXATK2_1:
            case S_PHOENIXATK2_2:
            case S_PHOENIXATK2_3:
            case S_PHOENIXATK2_4:
            {
                return fullbright;
                break;
            }
        }
	}

    return nobrightmap;
}

// [crispy] initialize brightmaps

void R_InitBrightmaps ()
{
    // [crispy] only five select brightmapped flats
    bmapflatnum[0] = R_FlatNumForName("FLOOR21");
    bmapflatnum[1] = R_FlatNumForName("FLOOR22");
    bmapflatnum[2] = R_FlatNumForName("FLOOR23");
    bmapflatnum[3] = R_FlatNumForName("FLOOR24");
    bmapflatnum[4] = R_FlatNumForName("FLOOR26");
}
