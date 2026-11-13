//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 1993-2008 Raven Software
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2005-2006 Florian Schulze, Colin Phipps, Neil Stevens, Andrey Budko
// Copyright(C) 2017 Fabian Greffrath
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
//	[crispy] support MUSINFO lump (dynamic music changing)
//

#include "g_umapinfo.h"
#include "m_scanner.h"
#include "s_sound.h"
#include "w_wad.h"
#include "z_zone.h"

#include "s_musinfo.h"

// [crispy] adapted from prboom-plus/src/s_advsound.c:54-159

musinfo_t musinfo = {0};

//
// S_ParseMusInfo
// Parses MUSINFO lump.
//

void S_ParseMusInfo(const char *mapid)
{
    scanner_t *s;
    int lumpnum = W_CheckNumForName("MUSINFO");
    if (lumpnum < 0)
    {
        return;
    }

    s = SCN_Open("MUSINFO", W_CacheLumpNum(lumpnum, PU_CACHE),
                W_LumpLength(lumpnum));

    while (SCN_TokensLeft(s))
    {
        if (SCN_CheckToken(s, TK_Identifier))
        {
            if (!strcasecmp(SCN_GetString(s), mapid))
            {
                break;
            }
        }
        else
        {
            SCN_GetNextLineToken(s);
        }
    }

    while (SCN_TokensLeft(s))
    {
        if (SCN_CheckToken(s, TK_Identifier))
        {
            if (G_ValidateMapName(SCN_GetString(s), NULL, NULL))
            {
                break;
            }
        }
        else if (SCN_CheckToken(s, TK_IntConst))
        {
            int num = SCN_GetNumber(s);
            // Check number in range
            if (num > 0 && num < MAX_MUS_ENTRIES)
            {
                SCN_GetNextRawString(s, true);
                lumpnum = W_CheckNumForName(SCN_GetString(s));
                if (lumpnum > 0)
                {
                    musinfo.items[num] = lumpnum;
                }
                else
                {
                    printf("S_ParseMusInfo: Unknown MUS lump %s\n",
                           SCN_GetString(s));
                }
            }
            else
            {
                printf("S_ParseMusInfo: Number not in range 1 to %d\n",
                       MAX_MUS_ENTRIES - 1);
            }
        }
        else
        {
            SCN_GetNextLineToken(s);
        }
    }

    SCN_Close(s);
}

void T_MusInfo (void)
{
  if (musinfo.tics < 0 || !musinfo.mapthing)
  {
    return;
  }

  if (musinfo.tics > 0)
  {
    musinfo.tics--;
  }
  else
  {
    if (!musinfo.tics && musinfo.lastmapthing != musinfo.mapthing)
    {
      // [crispy] encode music lump number in mapthing health
      int arraypt = musinfo.mapthing->health - 1000;

      if (arraypt >= 0 && arraypt < MAX_MUS_ENTRIES)
      {
        int lumpnum = musinfo.items[arraypt];

        if (lumpnum > 0 && lumpnum < numlumps)
        {
          S_ChangeMusInfoMusic(lumpnum, true);
        }
      }

      musinfo.tics = -1;
    }
  }
}
