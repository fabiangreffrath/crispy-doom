//
// Copyright(C) 1993-1996 Id Software, Inc.
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
#include "doomstat.h"
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
    fputs(line, save_stream);
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
        fputs(line, save_stream);
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

// flags2

static void P_WriteFlags2 (const char *key)
{
    thinker_t* th;

    for (th = thinkercap.next; th != &thinkercap; th=th->next)
    {
        if (th->function.acp1 == (actionf_p1)P_MobjThinker)
        {
            mobj_t *mo = (mobj_t *)th;

            if (mo->flags2)
            {
                M_snprintf(line, MAX_LINE_LEN, "%s %d %d\n",
                        key,
                        (uint32_t) P_ThinkerToIndex(th),
                        (int)mo->flags2);
                fputs(line, save_stream);
            }
        }
    }
}

static void P_ReadFlags2 (const char *key)
{
    int flags2;
    uint32_t index;

    if (sscanf(line, "%s %d %d\n",
               string,
               &index,
               &flags2) == 3 &&
        !strncmp(string, key, MAX_STRING_LEN))
    {
        mobj_t *mo = (mobj_t *) P_IndexToThinker(index);
        mo->flags2 = flags2;
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
    {"crispy-strife", P_WritePackageTarname, NULL, 0},
    {"markpoints", P_WriteMarkPoints, P_ReadMarkPoints, 1},
    {"flags2", P_WriteFlags2, P_ReadFlags2, 1},
};

void P_WriteExtendedSaveGameData ()
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
    while (fgets(line, MAX_LINE_LEN, save_stream))
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

void P_ReadExtendedSaveGameData ()
{
    line = malloc(MAX_LINE_LEN);
    string = malloc(MAX_STRING_LEN);

    // [crispy] second pass for Strife maps
    P_ReadKeyValuePairs(1);

    free(line);
    free(string);
}
