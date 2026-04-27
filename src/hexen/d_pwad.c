//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
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
//	Auto-loading of (semi-)official PWAD expansions, i.e.
//	Deathkings of the Dark Cidadel
//

#include <stdlib.h>

#include "h2def.h"
#include "crispy.h" 
#include "d_iwad.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "w_main.h"
#include "w_wad.h"
#include "w_merge.h" // [crispy] W_MergeFile()

extern char *iwadfile;

// [crispy] check if HEXDD.WAD is already loaded as a PWAD
static boolean CheckHexDDLoaded (void)
{
	int i, j;

	i = P_TranslateMap(1);
	j = P_TranslateMap(20);

	// Check if HexDD is already loaded by checking MapInfo Mapnames for warptarget 1 and 20
	if (i >= 0 && j >= 0 &&
		!strcasecmp(P_GetMapName(i), "RUINED VILLAGE") &&
	    !strcasecmp(P_GetMapName(j), "DARK CITADEL"))
	{
		// gamemission = pack_nerve;
		return true;
	}

	return false;
}

static const lump_rename_t dd_lumps [] = {
	{"TITLE", "TITLED"},
	{"CREDIT", "CREDITD"},
	{"HELP1",   "HELP1D"},
	{"HELP2",   "HELP2D"},
	{"DEMO1",    "DEMO1D"},
	{"DEMO2",    "DEMO2D"},
	{"DEMO3",    "DEMO3D"},
	{"MAPINFO",   "MAPINFOD"},
	{"CLUS1MSG",  "CLUS1MSD"},
	{"CLUS2MSG",  "CLUS2MSD"},
	{"WIN1MSG",   "WIN1MSGD"},
	{"WIN2MSG",   "WIN2MSGD"},
	{"WIN3MSG",   "WIN3MSGD"},
	{"SNDINFO",   "SNDINFOD"},
	{"M_HTIC",   "M_HTICD"},
};

// [crispy] auto-load HEXDD.WAD if available
static void CheckLoadHexDD (void)
{
	const char *dd_basename;
	char *autoload_dir;
	int i, j;

	// [crispy] don't load if another PWAD already provides MAP01
	i = W_CheckNumForName("MAP01");
	if (i != -1 && !W_IsIWADLump(lumpinfo[i]))
	{
		return;
	}

	if (strrchr(iwadfile, DIR_SEPARATOR) != NULL)
	{
		char *dir;
		dir = M_DirName(iwadfile);
		crispy->havedeathknights = M_StringJoin(dir, DIR_SEPARATOR_S, "HEXDD.WAD", NULL);
		free(dir);
	}
	else
	{
		crispy->havedeathknights = M_StringDuplicate("HEXDD.WAD");
	}

	if (!M_FileExists(crispy->havedeathknights))
	{
		free(crispy->havedeathknights);
		crispy->havedeathknights = D_FindWADByName("HEXDD.WAD");
		//crispy->havedeathknights = NULL;
	}

	if (crispy->havedeathknights == NULL)
	{
		return;
	}

	printf(" [Deathknights] adding %s\n", crispy->havedeathknights);
	W_AddFile(crispy->havedeathknights);
	dd_basename = M_BaseName(crispy->havedeathknights);

	// [crispy] add indicators to level
	for (i = 33; i <= 38; i++)
	{
		char lumpname[9];

		// M_snprintf(lumpname, 9, "CWILV%2.2d", i);
		// j = W_GetNumForName(lumpname);
		// if (!strcasecmp(W_WadNameForLump(lumpinfo[j]), master_basename))
		// {
		// 	lumpinfo[j]->name[0] = 'M';
		// }
		// else
		// {
		// 	// [crispy] indicate this is not the complete MASTERLEVELS.WAD
		// 	crispy->havemaster = (char *)-1;
		// }

		M_snprintf(lumpname, 9, "MAP%02d", i);
		j = W_GetNumForName(lumpname);
		strcat(lumpinfo[j]->name, "D");
	}

    // [crispy] rename intrusive DKDC graphics, demos and music lumps out
    // of the way
    for (i = 0; i < arrlen(dd_lumps); i++)
    {
        j = W_CheckNumForName(dd_lumps[i].name);

        if (j != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[j]), dd_basename))
        {
            memcpy(lumpinfo[j]->name, dd_lumps[i].new_name, 8);
        }
    }

	// // [crispy] if MASTERLEVELS.WAD contains TITLEPIC and INTERPIC, rename them
	// for (i = 0; i < arrlen(master_lumps); i++)
	// {
	// 	j = W_CheckNumForName(master_lumps[i].name);

	// 	if (j != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[j]), master_basename))
	// 	{
	// 		memcpy(lumpinfo[j]->name, master_lumps[i].new_name, 8);
	// 	}
	// }

	// [crispy] load WAD and DEH files from autoload directories
	if (!M_ParmExists("-noautoload"))
	{
		if ((autoload_dir = M_GetAutoloadDir(dd_basename, false)))
		{
			W_AutoLoadWADsRename(autoload_dir, dd_lumps, arrlen(dd_lumps));
			//DEH_AutoLoadPatches(autoload_dir);
			free(autoload_dir);
		}
	}

	// Consider also remaining points (see screenshot)

	// [crispy] regenerate the hashtable
	W_GenerateHashTable();

	return;
}

void D_LoadHexDD()
{
		// [crispy] check if HEXDD.WAD is already loaded as a PWAD
	if (!CheckHexDDLoaded())
	{
		// [crispy] else auto-load HEXDD.WAD if available
		CheckLoadHexDD();
	}
}
