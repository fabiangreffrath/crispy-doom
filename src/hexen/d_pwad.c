//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2026 Michael Balling
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

extern char *iwadfile;

// [crispy] check if HEXDD.WAD is already loaded as a PWAD
static boolean CheckHEXDDLoaded (void)
{
	int i, j;

	i = P_TranslateMap(1);
	j = P_TranslateMap(20);

	// Check if HEXDD.WAD is already loaded by checking MapInfo Mapnames for warptarget 1 and 20
	if (i >= 0 && j >= 0 &&
		!strcasecmp(P_GetMapName(i), "RUINED VILLAGE") &&
	    !strcasecmp(P_GetMapName(j), "DARK CITADEL"))
	{
		gameepisode = 2;
		return true;
	}

	return false;
}

static const lump_rename_t hexdd_lumps [] = {
	{"TITLE", 	 "TITLED"},
	{"HELP1",    "HELP1D"},
	{"HELP2",    "HELP2D"},
	{"DEMO1",    "DEMO1D"},
	{"DEMO2",    "DEMO2D"},
	{"DEMO3",    "DEMO3D"},
	{"M_HTIC",   "M_HTICD"},
	{"CREDIT",   "CREDITD"},
	{"MAPINFO",  "MAPINFOD"},
	{"CLUS1MSG", "CLUS1MSD"},
	{"CLUS2MSG", "CLUS2MSD"},
	{"WIN1MSG",  "WIN1MSGD"},
	{"WIN2MSG",  "WIN2MSGD"},
	{"WIN3MSG",  "WIN3MSGD"},
	{"SNDINFO",  "SNDINFOD"},
};

// [crispy] auto-load HEXDD.WAD if available
static void CheckLoadHEXDD (void)
{
	const char *dd_basename;
	char *autoload_dir;
	int i, j;

	// [crispy] don't load if another PWAD already provides MAP01 / MAPINFO
	i = W_CheckNumForName("MAP01");
	j = W_CheckNumForName("MAPINFO");
	if ((i != -1 && !W_IsIWADLump(lumpinfo[i])) ||
		(j != -1 && !W_IsIWADLump(lumpinfo[j])))
	{
		return;
	}

	if (strrchr(iwadfile, DIR_SEPARATOR) != NULL)
	{
		char *dir;
		dir = M_DirName(iwadfile);
		crispy->havedeathkings = M_StringJoin(dir, DIR_SEPARATOR_S, "HEXDD.WAD", NULL);
		free(dir);
	}
	else
	{
		crispy->havedeathkings = M_StringDuplicate("HEXDD.WAD");
	}

	if (!M_FileExists(crispy->havedeathkings))
	{
		free(crispy->havedeathkings);
		crispy->havedeathkings = D_FindWADByName("HEXDD.WAD");
	}

	if (crispy->havedeathkings == NULL)
	{
		return;
	}

	printf(" [Deathkings] adding %s\n", crispy->havedeathkings);
	W_AddFile(crispy->havedeathkings);
	dd_basename = M_BaseName(crispy->havedeathkings);

	// [crispy] add indicators to level
	for (i = 33; i <= 38; i++)
	{
		char lumpname[9];

		M_snprintf(lumpname, 9, "MAP%02d", i);
		j = W_GetNumForName(lumpname);
		strcat(lumpinfo[j]->name, "D");
	}

    // [crispy] rename intrusive Deathkings graphics, demos and music lumps out
    // of the way
    for (i = 0; i < arrlen(hexdd_lumps); i++)
    {
        j = W_CheckNumForName(hexdd_lumps[i].name);

        if (j != -1 && !strcasecmp(W_WadNameForLump(lumpinfo[j]), dd_basename))
        {
            memcpy(lumpinfo[j]->name, hexdd_lumps[i].new_name, 8);
        }
    }

	// [crispy] load WAD and DEH files from autoload directories
	if (!M_ParmExists("-noautoload"))
	{
		if ((autoload_dir = M_GetAutoloadDir(dd_basename, false)))
		{
			W_AutoLoadWADsRename(autoload_dir, hexdd_lumps, arrlen(hexdd_lumps));
			//DEH_AutoLoadPatches(autoload_dir);
			free(autoload_dir);
		}
	}

	// [crispy] regenerate the hashtable
	W_GenerateHashTable();

	return;
}

void D_LoadHEXDD()
{
	// [crispy] check if HEXDD.WAD is already loaded as a PWAD
	if (!CheckHEXDDLoaded())
	{
		// [crispy] else auto-load HEXDD.WAD if available
		CheckLoadHEXDD();
	}
}
