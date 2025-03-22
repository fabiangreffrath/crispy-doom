//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2016 Fabian Greffrath
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
//	[crispy] Archiving: Extended SaveGame I/O.
//

#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "doomtype.h"
#include "doomdef.h"
#include "deh_main.h"
#include "m_misc.h"
#include "p_extsaveg.h"
#include "p_local.h"
#include "z_zone.h"

#define MAX_LINE_LEN 260
#define MAX_STRING_LEN 80

static char *line, *string;

static void P_WritePackageTarname (const char *key)
{
	M_snprintf(line, MAX_LINE_LEN, "%s %s\n", key, PACKAGE_VERSION);
	fputs(line, SaveGameFP);
}

// markpoints[]

extern void AM_GetMarkPoints (int *n, long *p);
extern void AM_SetMarkPoints (int n, long *p);

static void P_WriteMarkPoints (const char *key)
{
	int n;
	long p[20];

	AM_GetMarkPoints(&n, p);

	if (p[0] != -1)
	{
		M_snprintf(line, MAX_LINE_LEN, "%s %d %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
		           key, n,
		           p[0], p[1], p[2], p[3], p[4],
		           p[5], p[6], p[7], p[8], p[9],
		           p[10], p[11], p[12], p[13], p[14],
		           p[15], p[16], p[17], p[18], p[19]);
		fputs(line, SaveGameFP);
	}
}

static void P_ReadMarkPoints (const char *key)
{
	int n;
	long p[20];

	if (sscanf(line, "%s %d %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n",
	           string, &n,
	           &p[0], &p[1], &p[2], &p[3], &p[4],
	           &p[5], &p[6], &p[7], &p[8], &p[9],
	           &p[10], &p[11], &p[12], &p[13], &p[14],
	           &p[15], &p[16], &p[17], &p[18], &p[19]) == 22 &&
	    !strncmp(string, key, MAX_STRING_LEN))
	{
		AM_SetMarkPoints(n, p);
	}
}

typedef struct
{
	const char *key;
	void (* extsavegwritefn) (const char *key);
	void (* extsavegreadfn) (const char *key);
	const int pass;
} extsavegdata_t;

static const extsavegdata_t extsavegdata[] =
{
	// [crispy] @FORKS: please change this if you are going to introduce incompatible changes!
	{"crispy-heretic", P_WritePackageTarname, NULL, 0},
	{"markpoints", P_WriteMarkPoints, P_ReadMarkPoints, 1},
};

void P_WriteExtendedSaveGameData (void)
{
	int i;

	line = malloc(MAX_LINE_LEN);

	for (i = 0; i < arrlen(extsavegdata); i++)
	{
		extsavegdata[i].extsavegwritefn(extsavegdata[i].key);
	}

	free(line);
}

static void P_ReadKeyValuePairs (int pass)
{
	while (fgets(line, MAX_LINE_LEN, SaveGameFP))
	{
		if (sscanf(line, "%s", string) == 1)
		{
			int i;

			for (i = 1; i < arrlen(extsavegdata); i++)
			{
				if (extsavegdata[i].extsavegreadfn &&
				    extsavegdata[i].pass == pass &&
				    !strncmp(string, extsavegdata[i].key, MAX_STRING_LEN))
				{
					extsavegdata[i].extsavegreadfn(extsavegdata[i].key);
				}
			}
		}
	}
}

// [crispy] pointer to the info struct for the map lump about to load
lumpinfo_t *savemaplumpinfo = NULL;

void P_ReadExtendedSaveGameData (int pass)
{
	long p, curpos, endpos;
	byte episode, map;
	int lumpnum = -1;

	line = malloc(MAX_LINE_LEN);
	string = malloc(MAX_STRING_LEN);

	// [crispy] two-pass reading of extended savegame data
	if (pass == 1)
	{
		P_ReadKeyValuePairs(1);

		free(line);
		free(string);

		return;
	}

	curpos = ftell(SaveGameFP);

	// // [crispy] check which map we would want to load
	// fseek(SaveGameFP, SAVESTRINGSIZE + VERSIONSIZE + 1, SEEK_SET); // [crispy] + 1 for "gameskill"
	// if (fread(&episode, 1, 1, SaveGameFP) == 1 &&
	//     fread(&map, 1, 1, SaveGameFP) == 1)
	// {
	// 	lumpnum = P_GetNumForMap ((int) episode, (int) map, false);
	// }

	// if (lumpnum >= 0)
	// {
	// 	savemaplumpinfo = lumpinfo[lumpnum];
	// }
	// else
	// {
	// 	// [crispy] unavailable map!
	// 	savemaplumpinfo = NULL;
	// }

	// [crispy] read key/value pairs past the end of the regular savegame data
	fseek(SaveGameFP, 0, SEEK_END);
	endpos = ftell(SaveGameFP);

	for (p = endpos - 1; p > 0; p--)
	{
		byte curbyte;

		fseek(SaveGameFP, p, SEEK_SET);

		if (fread(&curbyte, 1, 1, SaveGameFP) < 1)
		{
			break;
		}

		if (curbyte == SAVE_GAME_TERMINATOR)
		{
			if (!fgets(line, MAX_LINE_LEN, SaveGameFP))
			{
				continue;
			}

			if (sscanf(line, "%s", string) == 1 &&
			    !strncmp(string, extsavegdata[0].key, MAX_STRING_LEN))
			{
				P_ReadKeyValuePairs(0);
				break;
			}
		}
	}

	free(line);
	free(string);

	// [crispy] back to where we started
	fseek(SaveGameFP, curpos, SEEK_SET);
}
