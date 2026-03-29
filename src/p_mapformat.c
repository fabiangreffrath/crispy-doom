//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2015-2018 Fabian Greffrath
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

#include <stdio.h>
#include <string.h>
#include "doomtype.h"
#include "m_argv.h"
#include "p_mapformat.h"
#include "w_wad.h"
#include "z_zone.h"

// [crispy] check map format based on level lumps
enum
{
    LL_LABEL,    // A separator, name, ExMx or MAPxx
    LL_THINGS,   // Monsters, items..
    LL_LINEDEFS, // LineDefs, from editing
    LL_SIDEDEFS, // SideDefs, from editing
    LL_VERTEXES, // Vertices, edited and BSP splits generated
    LL_SEGS,     // LineSegs, from LineDefs split by BSP
    LL_SSECTORS, // SubSectors, list of LineSegs
    LL_NODES,    // BSP nodes
    LL_SECTORS,  // Sectors, from editing
    LL_REJECT,   // LUT, sector-sector visibility	
    LL_BLOCKMAP, // LUT, motion clipping, walls/grid element
    LL_BEHAVIOR, // ACS bytecode
};

static char* bsp_names[] = {
    [NFMT_DOOMBSP] = "DoomBSP",
    [NFMT_DEEPBSPV4] = "DeePBSPV4",
    [NFMT_XNOD] = "XNOD",
    [NFMT_XGLN] = "XGLN",
    [NFMT_XGL2] = "XGL2",
    [NFMT_XGL3] = "XGL3",
};

// [crispy] check for extended BSP tree formats
mapformat_t P_CheckMapFormat (int lumpnum)
{
    byte *lump_data = NULL;
    int size_subs = 0, size_nodes = 0;
    mapformat_t format = {
        .bsp = NFMT_DOOMBSP,
        .compressed = false,
        .hexen = false,
    };

    if (W_LumpExistsWithName(lumpnum + LL_BEHAVIOR, "BEHAVIOR"))
    {
        format.hexen = true;
    }

    //!
    // @category mod
    //
    // Forces extended (non-GL) ZDoom nodes.
    //

    if (!M_CheckParm("-force_old_zdoom_nodes"))
    {
    	// Check for XGL BSP tree in LL_SSECTORS
        size_subs = W_LumpLengthWithName(lumpnum + LL_SSECTORS, "SSECTORS");

        if (size_subs >= sizeof(mapsubsector_t))
        {
            lump_data = W_CacheLumpNum(lumpnum + LL_SSECTORS, PU_STATIC);

            if (!memcmp(lump_data, "XGLN", 4))
            {
                format.bsp = NFMT_XGLN;
            }
            else if (!memcmp(lump_data, "XGL2", 4))
            {
                format.bsp = NFMT_XGL2;
            }
            else if (!memcmp(lump_data, "XGL3", 4))
            {
                format.bsp = NFMT_XGL3;
            }
            else if (!memcmp(lump_data, "ZGLN", 4))
            {
                format.bsp = NFMT_XGLN;
                format.compressed = true;
            }
            else if (!memcmp(lump_data, "ZGL2", 4))
            {
                format.bsp = NFMT_XGL2;
                format.compressed = true;
            }
            else if (!memcmp(lump_data, "ZGL3", 4))
            {
                format.bsp = NFMT_XGL3;
                format.compressed = true;
            }
        }

        if (lump_data)
        {
            W_ReleaseLumpNum(lumpnum + LL_SSECTORS);
            lump_data = NULL;
        }
    }

    if (format.bsp == NFMT_DOOMBSP)
    {
    	// Did not find XGL tree? Check other formats
        size_nodes = W_LumpLengthWithName(lumpnum + LL_NODES, "NODES");

        if (size_nodes >= sizeof(mapnode_t))
        {
            lump_data = W_CacheLumpNum(lumpnum + LL_NODES, PU_STATIC);

            if (!memcmp(lump_data, "xNd4\0\0\0\0", 8))
            {
                format.bsp = NFMT_DEEPBSPV4;
            }
            else if (!memcmp(lump_data, "XNOD", 4))
            {
                format.bsp = NFMT_XNOD;
            }
            else if (!memcmp(lump_data, "ZNOD", 4))
            {
                format.bsp = NFMT_XNOD;
                format.compressed = true;
            }
        }

        if (lump_data)
        {
            W_ReleaseLumpNum(lumpnum + LL_NODES);
            lump_data = NULL;
        }
    }

    fprintf(stderr, "P_CheckMapFormat: [%s] %s map format - %s%s node format\n",
            lumpinfo[lumpnum]->name,
            format.hexen ? "Hexen" : "Doom",
            format.compressed ? "compressed " : "",
            bsp_names[format.bsp]
            );

    return format;
}
