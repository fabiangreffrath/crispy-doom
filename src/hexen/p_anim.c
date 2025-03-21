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


// HEADER FILES ------------------------------------------------------------

#include "h2def.h"
#include "m_random.h"
#include "i_system.h"
#include "p_local.h"
#include "s_sound.h"
#include "a11y.h" // [crispy] A11Y

// MACROS ------------------------------------------------------------------

#define ANIM_SCRIPT_NAME "ANIMDEFS"
#define MAX_ANIM_DEFS 20
#define MAX_FRAME_DEFS 96
#define ANIM_FLAT 0
#define ANIM_TEXTURE 1
#define SCI_FLAT "flat"
#define SCI_TEXTURE "texture"
#define SCI_PIC "pic"
#define SCI_TICS "tics"
#define SCI_RAND "rand"

#define LIGHTNING_SPECIAL 	198
#define LIGHTNING_SPECIAL2 	199
#define SKYCHANGE_SPECIAL 	200

#define TEXTURE_SCROLL_SHIFT    10 // [crispy]

// TYPES -------------------------------------------------------------------

typedef struct
{
    int index;
    int tics;
} frameDef_t;

typedef struct
{
    int type;
    int index;
    int tics;
    int currentFrameDef;
    int startFrameDef;
    int endFrameDef;
} animDef_t;

// EXTERNAL FUNCTION PROTOTYPES --------------------------------------------

// PUBLIC FUNCTION PROTOTYPES ----------------------------------------------

// PRIVATE FUNCTION PROTOTYPES ---------------------------------------------

static void P_LightningFlash(void);

// EXTERNAL DATA DECLARATIONS ----------------------------------------------


// PUBLIC DATA DEFINITIONS -------------------------------------------------

fixed_t Sky1ScrollDelta;
fixed_t Sky2ScrollDelta;

// PRIVATE DATA DEFINITIONS ------------------------------------------------

static animDef_t AnimDefs[MAX_ANIM_DEFS];
static frameDef_t FrameDefs[MAX_FRAME_DEFS];
static int AnimDefCount;
static boolean LevelHasLightning;
static int NextLightningFlash;
static int LightningFlash;
static int *LightningLightLevels;

// CODE --------------------------------------------------------------------

//==========================================================================
//
// P_AnimateSurfaces
//
//==========================================================================

void P_AnimateSurfaces(void)
{
    int i;
    animDef_t *ad;
    line_t *line;

    // Animate flats and textures
    for (i = 0; i < AnimDefCount; i++)
    {
        ad = &AnimDefs[i];
        ad->tics--;
        if (ad->tics == 0)
        {
            if (ad->currentFrameDef == ad->endFrameDef)
            {
                ad->currentFrameDef = ad->startFrameDef;
            }
            else
            {
                ad->currentFrameDef++;
            }
            ad->tics = FrameDefs[ad->currentFrameDef].tics;
            if (ad->tics > 255)
            {                   // Random tics
                ad->tics = (ad->tics >> 16)
                    + P_Random() % ((ad->tics & 0xff00) >> 8);
            }
            if (ad->type == ANIM_FLAT)
            {
                flattranslation[ad->index] =
                    FrameDefs[ad->currentFrameDef].index;
            }
            else
            {                   // Texture
                texturetranslation[ad->index] =
                    FrameDefs[ad->currentFrameDef].index;
            }
        }
    }

    // Update scrolling textures
    for (i = 0; i < numlinespecials; i++)
    {
        line = linespeciallist[i];
        switch (line->special)
        {
            case 100:          // Scroll_Texture_Left
                sides[line->sidenum[0]].basetextureoffset +=
                    line->arg1 << TEXTURE_SCROLL_SHIFT;
                sides[line->sidenum[0]].textureoffset =
                    sides[line->sidenum[0]].basetextureoffset;
                break;
            case 101:          // Scroll_Texture_Right
                sides[line->sidenum[0]].basetextureoffset -=
                    line->arg1 << TEXTURE_SCROLL_SHIFT;
                sides[line->sidenum[0]].textureoffset =
                    sides[line->sidenum[0]].basetextureoffset;
                break;
            case 102:          // Scroll_Texture_Up
                sides[line->sidenum[0]].baserowoffset +=
                    line->arg1 << TEXTURE_SCROLL_SHIFT;
                sides[line->sidenum[0]].rowoffset =
                    sides[line->sidenum[0]].baserowoffset;
                break;
            case 103:          // Scroll_Texture_Down
                sides[line->sidenum[0]].baserowoffset -=
                    line->arg1 << TEXTURE_SCROLL_SHIFT;
                sides[line->sidenum[0]].rowoffset =
                    sides[line->sidenum[0]].baserowoffset;
                break;
        }
    }

    // Update sky column offsets
    Sky1ColumnOffset += Sky1ScrollDelta;
    Sky2ColumnOffset += Sky2ScrollDelta;

    if (LevelHasLightning)
    {
        if (!NextLightningFlash || LightningFlash)
        {
            P_LightningFlash();
        }
        else
        {
            NextLightningFlash--;
        }
    }
}

// [crispy] smooth texture scrolling
void R_InterpolateTextureOffsets(void)
{
    const line_t* line;
    side_t* side;
    int i;

    if (!(crispy->uncapped && leveltime > oldleveltime))
    {
        return;
    }

    for (i = 0; i < numlinespecials; i++)
    {
        line = linespeciallist[i];
        side = &sides[line->sidenum[0]];

        switch (line->special)
        {
            case 100:       // Scroll_Texture_Right
                side->textureoffset = side->basetextureoffset +
                    (FixedMul(fractionaltic, line->arg1) << TEXTURE_SCROLL_SHIFT);
                break;
            case 101:       // Scroll_Texture_Left
                side->textureoffset = side->basetextureoffset -
                    (FixedMul(fractionaltic, line->arg1) << TEXTURE_SCROLL_SHIFT);
                break;
            case 102:       // Scroll_Texture_Up
                side->rowoffset = side->baserowoffset +
                    (FixedMul(fractionaltic, line->arg1) << TEXTURE_SCROLL_SHIFT);
                break;
            case 103:       // Scroll_Texture_Down
                side->rowoffset = side->baserowoffset -
                    (FixedMul(fractionaltic, line->arg1) << TEXTURE_SCROLL_SHIFT);
                break;
        }
    }
}
//==========================================================================
//
// P_LightningFlash
//
//==========================================================================

static void P_LightningFlash(void)
{
    int i;
    sector_t *tempSec;
    int *tempLight;
    boolean foundSec;
    int flashLight;

    if (LightningFlash)
    {
        LightningFlash--;
        if (LightningFlash)
        {
            tempLight = LightningLightLevels;
            tempSec = sectors;
            for (i = 0; i < numsectors; i++, tempSec++)
            {
                if (tempSec->ceilingpic == skyflatnum
                    || tempSec->special == LIGHTNING_SPECIAL
                    || tempSec->special == LIGHTNING_SPECIAL2)
                {
                    if (*tempLight < tempSec->lightlevel - 4)
                    {
                        tempSec->lightlevel -= 4;
                    }
                    tempLight++;
                }
                if (a11y_sector_lighting)
                    tempSec->rlightlevel = tempSec->lightlevel; // [crispy] A11Y
            }
        }
        else
        {                       // remove the alternate lightning flash special
            tempLight = LightningLightLevels;
            tempSec = sectors;
            for (i = 0; i < numsectors; i++, tempSec++)
            {
                if (tempSec->ceilingpic == skyflatnum
                    || tempSec->special == LIGHTNING_SPECIAL
                    || tempSec->special == LIGHTNING_SPECIAL2)
                {
                    tempSec->lightlevel = *tempLight;
                    tempLight++;
                }
                if (a11y_sector_lighting)
                    tempSec->rlightlevel = tempSec->lightlevel; // [crispy] A11Y
            }
            Sky1Texture = P_GetMapSky1Texture(gamemap);
        }
        return;
    }
    LightningFlash = (P_Random() & 7) + 8;
    flashLight = 200 + (P_Random() & 31);
    tempSec = sectors;
    tempLight = LightningLightLevels;
    foundSec = false;
    for (i = 0; i < numsectors; i++, tempSec++)
    {
        if (tempSec->ceilingpic == skyflatnum
            || tempSec->special == LIGHTNING_SPECIAL
            || tempSec->special == LIGHTNING_SPECIAL2)
        {
            *tempLight = tempSec->lightlevel;
            if (tempSec->special == LIGHTNING_SPECIAL)
            {
                tempSec->lightlevel += 64;
                if (tempSec->lightlevel > flashLight)
                {
                    tempSec->lightlevel = flashLight;
                }
            }
            else if (tempSec->special == LIGHTNING_SPECIAL2)
            {
                tempSec->lightlevel += 32;
                if (tempSec->lightlevel > flashLight)
                {
                    tempSec->lightlevel = flashLight;
                }
            }
            else
            {
                tempSec->lightlevel = flashLight;
            }
            if (tempSec->lightlevel < *tempLight)
            {
                tempSec->lightlevel = *tempLight;
            }
            tempLight++;
            foundSec = true;
        }
        if (a11y_sector_lighting)
            tempSec->rlightlevel = tempSec->lightlevel; // [crispy] A11Y
    }
    if (foundSec)
    {
        Sky1Texture = P_GetMapSky2Texture(gamemap);     // set alternate sky                
        S_StartSound(NULL, SFX_THUNDER_CRASH);
    }
    // Calculate the next lighting flash
    if (!NextLightningFlash)
    {
        if (P_Random() < 50)
        {                       // Immediate Quick flash
            NextLightningFlash = (P_Random() & 15) + 16;
        }
        else
        {
            if (P_Random() < 128 && !(leveltime & 32))
            {
                NextLightningFlash = ((P_Random() & 7) + 2) * 35;
            }
            else
            {
                NextLightningFlash = ((P_Random() & 15) + 5) * 35;
            }
        }
    }
}

//==========================================================================
//
// P_ForceLightning
//
//==========================================================================

void P_ForceLightning(void)
{
    NextLightningFlash = 0;
}

//==========================================================================
//
// P_InitLightning
//
//==========================================================================

void P_InitLightning(void)
{
    int i;
    int secCount;

    if (!P_GetMapLightning(gamemap))
    {
        LevelHasLightning = false;
        LightningFlash = 0;
        return;
    }
    LightningFlash = 0;
    secCount = 0;
    for (i = 0; i < numsectors; i++)
    {
        if (sectors[i].ceilingpic == skyflatnum
            || sectors[i].special == LIGHTNING_SPECIAL
            || sectors[i].special == LIGHTNING_SPECIAL2)
        {
            secCount++;
        }
    }
    if (secCount)
    {
        LevelHasLightning = true;
    }
    else
    {
        LevelHasLightning = false;
        return;
    }
    LightningLightLevels = (int *) Z_Malloc(secCount * sizeof(int), PU_LEVEL,
                                            NULL);
    NextLightningFlash = ((P_Random() & 15) + 5) * 35;  // don't flash at level start
}

//==========================================================================
//
// P_InitFTAnims
//
// Initialize flat and texture animation lists.
//
//==========================================================================

void P_InitFTAnims(void)
{
    int base;
    int mod;
    int fd;
    animDef_t *ad;
    boolean ignore;
    boolean done;

    fd = 0;
    ad = AnimDefs;
    AnimDefCount = 0;
    SC_Open(ANIM_SCRIPT_NAME);
    while (SC_GetString())
    {
        if (AnimDefCount == MAX_ANIM_DEFS)
        {
            I_Error("P_InitFTAnims: too many AnimDefs.");
        }
        if (SC_Compare(SCI_FLAT))
        {
            ad->type = ANIM_FLAT;
        }
        else if (SC_Compare(SCI_TEXTURE))
        {
            ad->type = ANIM_TEXTURE;
        }
        else
        {
            SC_ScriptError(NULL);
        }
        SC_MustGetString();     // Name
        ignore = false;
        if (ad->type == ANIM_FLAT)
        {
            if (W_CheckNumForName(sc_String) == -1)
            {
                ignore = true;
            }
            else
            {
                ad->index = R_FlatNumForName(sc_String);
            }
        }
        else
        {                       // Texture
            if (R_CheckTextureNumForName(sc_String) == -1)
            {
                ignore = true;
            }
            else
            {
                ad->index = R_TextureNumForName(sc_String);
            }
        }
        ad->startFrameDef = fd;
        done = false;
        while (done == false)
        {
            if (SC_GetString())
            {
                if (SC_Compare(SCI_PIC))
                {
                    if (fd == MAX_FRAME_DEFS)
                    {
                        I_Error("P_InitFTAnims: too many FrameDefs.");
                    }
                    SC_MustGetNumber();
                    if (ignore == false)
                    {
                        FrameDefs[fd].index = ad->index + sc_Number - 1;
                    }
                    SC_MustGetString();
                    if (SC_Compare(SCI_TICS))
                    {
                        SC_MustGetNumber();
                        if (ignore == false)
                        {
                            FrameDefs[fd].tics = sc_Number;
                            fd++;
                        }
                    }
                    else if (SC_Compare(SCI_RAND))
                    {
                        SC_MustGetNumber();
                        base = sc_Number;
                        SC_MustGetNumber();
                        if (ignore == false)
                        {
                            mod = sc_Number - base + 1;
                            FrameDefs[fd].tics = (base << 16) + (mod << 8);
                            fd++;
                        }
                    }
                    else
                    {
                        SC_ScriptError(NULL);
                    }
                }
                else
                {
                    SC_UnGet();
                    done = true;
                }
            }
            else
            {
                done = true;
            }
        }
        if ((ignore == false) && (fd - ad->startFrameDef < 2))
        {
            I_Error("P_InitFTAnims: AnimDef has framecount < 2.");
        }
        if (ignore == false)
        {
            ad->endFrameDef = fd - 1;
            ad->currentFrameDef = ad->endFrameDef;
            ad->tics = 1;       // Force 1st game tic to animate
            AnimDefCount++;
            ad++;
        }
    }
    SC_Close();
}
