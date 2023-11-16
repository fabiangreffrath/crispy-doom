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
// F_finale.c

#include <ctype.h>

#include "doomdef.h"
#include "deh_str.h"
#include "i_swap.h"
#include "i_video.h"
#include "s_sound.h"
#include "v_video.h"
#include "am_map.h"

static int finalestage;                // 0 = text, 1 = art screen
static int finalecount;

#define TEXTSPEED       3
#define TEXTWAIT        250

static const char *finaletext;
static const char *finaleflat;

static int FontABaseLump;


/*
=======================
=
= F_StartFinale
=
=======================
*/

void F_StartFinale(void)
{
    gameaction = ga_nothing;
    gamestate = GS_FINALE;
    viewactive = false;
    automapactive = false;
    players[consoleplayer].messageTics = 1;
    players[consoleplayer].message = NULL;
    players[consoleplayer].centerMessage = NULL;

    switch (gameepisode)
    {
        case 1:
            finaleflat = DEH_String("FLOOR25");
            finaletext = DEH_String(E1TEXT);
            break;
        case 2:
            finaleflat = DEH_String("FLATHUH1");
            finaletext = DEH_String(E2TEXT);
            break;
        case 3:
            finaleflat = DEH_String("FLTWAWA2");
            finaletext = DEH_String(E3TEXT);
            break;
        case 4:
            finaleflat = DEH_String("FLOOR28");
            finaletext = DEH_String(E4TEXT);
            break;
        case 5:
            finaleflat = DEH_String("FLOOR08");
            finaletext = DEH_String(E5TEXT);
            break;
    }

    finalestage = 0;
    finalecount = 0;
    FontABaseLump = W_GetNumForName(DEH_String("FONTA_S")) + 1;

//      S_ChangeMusic(mus_victor, true);
    S_StartSong(mus_cptd, true);
}



boolean F_Responder(event_t * event)
{
    if (event->type != ev_keydown)
    {
        return false;
    }
    if (finalestage == 1 && gameepisode == 2)
    {                           // we're showing the water pic, make any key kick to demo mode
        finalestage++;
        /*
        memset((byte *) 0xa0000, 0, SCREENWIDTH * SCREENHEIGHT);
        memset(I_VideoBuffer, 0, SCREENWIDTH * SCREENHEIGHT);
        I_SetPalette(W_CacheLumpName("PLAYPAL", PU_CACHE));
        */
        return true;
    }
    return false;
}


/*
=======================
=
= F_Ticker
=
=======================
*/

void F_Ticker(void)
{
    finalecount++;
    if (!finalestage
        && finalecount > strlen(finaletext) * TEXTSPEED + TEXTWAIT)
    {
        finalecount = 0;
        if (!finalestage)
        {
            finalestage = 1;
        }

//              wipegamestate = -1;             // force a wipe
/*
		if (gameepisode == 3)
			S_StartMusic (mus_bunny);
*/
    }
}


/*
=======================
=
= F_TextWrite
=
=======================
*/


void F_TextWrite(void)
{
    byte *src;
    pixel_t *dest;
    int x, y;
    int count;
    const char *ch;
    int c;
    int cx, cy;
    patch_t *w;

//
// erase the entire screen to a tiled background
//
    src = W_CacheLumpName(finaleflat, PU_CACHE);
    dest = I_VideoBuffer;
    for (y = 0; y < SCREENHEIGHT; y++)
    {
#ifndef CRISPY_TRUECOLOR
        for (x = 0; x < SCREENWIDTH / 64; x++)
        {
            memcpy(dest, src + ((y & 63) << 6), 64);
            dest += 64;
        }
        if (SCREENWIDTH & 63)
        {
            memcpy(dest, src + ((y & 63) << 6), SCREENWIDTH & 63);
            dest += (SCREENWIDTH & 63);
        }
#else
        for (x = 0; x < SCREENWIDTH; x++)
        {
            *dest++ = colormaps[src[((y & 63) << 6) + (x & 63)]];
        }
#endif
    }

//      V_MarkRect (0, 0, SCREENWIDTH, SCREENHEIGHT);

//
// draw some of the text onto the screen
//
    cx = 20;
    cy = 5;
    ch = finaletext;

    count = (finalecount - 10) / TEXTSPEED;
    if (count < 0)
        count = 0;
    for (; count; count--)
    {
        c = *ch++;
        if (!c)
            break;
        if (c == '\n')
        {
            cx = 20;
            cy += 9;
            continue;
        }

        c = toupper(c);
        if (c < 33)
        {
            cx += 5;
            continue;
        }

        w = W_CacheLumpNum(FontABaseLump + c - 33, PU_CACHE);
        if (cx + SHORT(w->width) > SCREENWIDTH)
            break;
        V_DrawPatch(cx, cy, w);
        cx += SHORT(w->width);
    }

}


void F_DrawPatchCol(int x, patch_t * patch, int col)
{
    column_t *column;
    byte *source;
    pixel_t *dest, *desttop;
    int count;

    column = (column_t *) ((byte *) patch + LONG(patch->columnofs[col]));
    desttop = I_VideoBuffer + x;

// step through the posts in a column

    while (column->topdelta != 0xff)
    {
        source = (byte *) column + 3;
        dest = desttop + column->topdelta * SCREENWIDTH;
        count = column->length;

        while (count--)
        {
            *dest = *source++;
            dest += SCREENWIDTH;
        }
        column = (column_t *) ((byte *) column + column->length + 4);
    }
}

/*
==================
=
= F_DemonScroll
=
==================
*/

void F_DemonScroll(void)
{
    byte *p1, *p2;
    static int yval = 0;
    static int yval_dest = 0; // [crispy]
    static int nextscroll = 0;
    lumpindex_t i1, i2; // [crispy]
    int x; // [crispy]
    patch_t *patch1, *patch2; // [crispy]
    static int y = 0; // [crispy]

    if (finalecount < nextscroll)
    {
        return;
    }
    i1 = W_GetNumForName(DEH_String("FINAL1"));
    i2 = W_GetNumForName(DEH_String("FINAL2"));
    p1 = W_CacheLumpNum(i1, PU_LEVEL);
    p2 = W_CacheLumpNum(i2, PU_LEVEL);
    if (finalecount < 70)
    {
        V_DrawFullscreenRawOrPatch(i1);
        nextscroll = finalecount;
        return;
    }
    if (yval < 64000)
    {
        if ((W_LumpLength(i1) == 64000) && (W_LumpLength(i2) == 64000))
        {
            V_CopyScaledBuffer(I_VideoBuffer, p2 + ORIGHEIGHT * ORIGWIDTH - yval, yval);
            V_CopyScaledBuffer(I_VideoBuffer + yval_dest, p1, ORIGHEIGHT * ORIGWIDTH - yval);

            yval_dest += SCREENWIDTH << crispy->hires;
        }
        else // [crispy] assume that FINAL1 and FINAL2 are in patch format
        {
            patch1 = (patch_t *)p1;
            patch2 = (patch_t *)p2;

            x = ((SCREENWIDTH >> crispy->hires) - SHORT(patch1->width)) / 2
                - WIDESCREENDELTA;

            // [crispy] pillar boxing
            if (x > -WIDESCREENDELTA)
            {
                V_DrawFilledBox(0, 0, WIDESCREENDELTA + x, SCREENHEIGHT, 0);
                V_DrawFilledBox(SCREENWIDTH - WIDESCREENDELTA - x, 0,
                                WIDESCREENDELTA + x, SCREENHEIGHT, 0);
            }

            V_DrawPatch(x, y - 200, patch2);
            V_DrawPatch(x, 0 + y, patch1);
            y++;
        }
        yval += ORIGWIDTH;
        nextscroll = finalecount + 3;
    }
    else
    {                           //else, we'll just sit here and wait, for now
        V_DrawFullscreenRawOrPatch(i2);
    }
}

/*
==================
=
= F_DrawUnderwater
=
==================
*/

void F_DrawUnderwater(void)
{
    static boolean underwawa = false;
#ifndef CRISPY_TRUECOLOR
    const char *lumpname;
    byte *palette;
#endif

    // The underwater screen has its own palette, which is rather annoying.
    // The palette doesn't correspond to the normal palette. Because of
    // this, we must regenerate the lookup tables used in the video scaling
    // code.

    switch (finalestage)
    {
        case 1:
            if (!underwawa)
            {
                underwawa = true;
                V_DrawFilledBox(0, 0, SCREENWIDTH, SCREENHEIGHT, 0);
#ifndef CRISPY_TRUECOLOR
                lumpname = DEH_String("E2PAL");
                palette = W_CacheLumpName(lumpname, PU_STATIC);
                I_SetPalette(palette);
                W_ReleaseLumpName(lumpname);
#else
                // [JN] TODO - E2END using different palette
                I_SetPalette(0);
#endif
                V_DrawFullscreenRawOrPatch(W_GetNumForName(DEH_String("E2END")));
            }
            paused = false;
            MenuActive = false;
            askforquit = false;

            break;
        case 2:
            if (underwawa)
            {
#ifndef CRISPY_TRUECOLOR
                lumpname = DEH_String("PLAYPAL");
                palette = W_CacheLumpName(lumpname, PU_STATIC);
                I_SetPalette(palette);
                W_ReleaseLumpName(lumpname);
#else
                // [JN] TODO - E2END using different palette
                I_SetPalette(0);
#endif
                underwawa = false;
            }
            V_DrawFullscreenRawOrPatch(W_GetNumForName(DEH_String("TITLE")));
            //D_StartTitle(); // go to intro/demo mode.
    }
}


#if 0
/*
==================
=
= F_BunnyScroll
=
==================
*/

void F_BunnyScroll(void)
{
    int scrolled, x;
    patch_t *p1, *p2;
    char name[10];
    int stage;
    static int laststage;

    p1 = W_CacheLumpName("PFUB2", PU_LEVEL);
    p2 = W_CacheLumpName("PFUB1", PU_LEVEL);

    V_MarkRect(0, 0, SCREENWIDTH, SCREENHEIGHT);

    scrolled = 320 - (finalecount - 230) / 2;
    if (scrolled > 320)
        scrolled = 320;
    if (scrolled < 0)
        scrolled = 0;

    for (x = 0; x < SCREENWIDTH; x++)
    {
        if (x + scrolled < 320)
            F_DrawPatchCol(x, p1, x + scrolled);
        else
            F_DrawPatchCol(x, p2, x + scrolled - 320);
    }

    if (finalecount < 1130)
        return;
    if (finalecount < 1180)
    {
        V_DrawPatch((SCREENWIDTH - 13 * 8) / 2, (SCREENHEIGHT - 8 * 8) / 2, 0,
                    W_CacheLumpName("END0", PU_CACHE));
        laststage = 0;
        return;
    }

    stage = (finalecount - 1180) / 5;
    if (stage > 6)
        stage = 6;
    if (stage > laststage)
    {
        S_StartSound(NULL, sfx_pistol);
        laststage = stage;
    }

    M_snprintf(name, sizeof(name), "END%i", stage);
    V_DrawPatch((SCREENWIDTH - 13 * 8) / 2, (SCREENHEIGHT - 8 * 8) / 2,
                W_CacheLumpName(name, PU_CACHE));
}
#endif

/*
=======================
=
= F_Drawer
=
=======================
*/

void F_Drawer(void)
{
    UpdateState |= I_FULLSCRN;
    if (!finalestage)
        F_TextWrite();
    else
    {
        switch (gameepisode)
        {
            case 1:
                if (gamemode == shareware)
                {
                    V_DrawFullscreenRawOrPatch(W_GetNumForName("ORDER"));
                }
                else
                {
                    V_DrawFullscreenRawOrPatch(W_GetNumForName("CREDIT"));
                }
                break;
            case 2:
                F_DrawUnderwater();
                break;
            case 3:
                F_DemonScroll();
                break;
            case 4:            // Just show credits screen for extended episodes
            case 5:
                V_DrawFullscreenRawOrPatch(W_GetNumForName("CREDIT"));
                break;
        }
    }
}
