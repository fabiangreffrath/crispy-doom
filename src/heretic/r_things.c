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
// R_things.c
#include <stdio.h>
#include <stdlib.h>
#include "doomdef.h"
#include "deh_str.h"
#include "i_swap.h"
#include "i_system.h"
#include "r_bmaps.h"
#include "r_local.h"
#include "v_trans.h" // [crispy] blending functions
#include "a11y.h" // [crispy] A11Y

typedef struct
{
    int x1, x2;

    int column;
    int topclip;
    int bottomclip;
} maskdraw_t;

typedef enum
{
    SPR_GWND_F1,
    SPR_GWND_F2,
    SPR_GWND_F3,
    SPR_BLSR_F1,
    SPR_BLSR_F2,
    SPR_BLSR_F3,
    SPR_HROD_F1,
    SPR_HROD_F2_5,
    SPR_HROD_F6,
    SPR_PHNX_F1,
    SPR_PHNX_F2,
    SPR_PHNX_F3, 
    NUMSOFFSETS
} spriteoffsetenum_t;

typedef struct
{
    spriteoffsetenum_t sprite;
    int offset;
} spriteoffset_t;

/*

Sprite rotation 0 is facing the viewer, rotation 1 is one angle turn CLOCKWISE around the axis.
This is not the same as the angle, which increases counter clockwise
(protractor).  There was a lot of stuff grabbed wrong, so I changed it...

*/


fixed_t pspritescale, pspriteiscale;

lighttable_t **spritelights;

// constant arrays used for psprite clipping and initializing clipping
int negonearray[MAXWIDTH];       // [crispy] 32-bit integer math
int screenheightarray[MAXWIDTH]; // [crispy] 32-bit integer math

/*
===============================================================================

						INITIALIZATION FUNCTIONS

===============================================================================
*/

// variables used to look up and range check thing_t sprites patches
spritedef_t *sprites;
int numsprites;

spriteframe_t sprtemp[26];
int maxframe;
const char *spritename;


// [crispy] Y-Offsets for various sprite frames used for weapon fire translucency
spriteoffset_t spriteoffsets[NUMSOFFSETS] = {
    {SPR_GWND_F1, 0 * FRACUNIT},
    {SPR_GWND_F2, 8 * FRACUNIT},
    {SPR_GWND_F3, 4 * FRACUNIT},
    {SPR_BLSR_F1, 0 * FRACUNIT},
    {SPR_BLSR_F2, 1 * FRACUNIT},
    {SPR_BLSR_F3, 4 * FRACUNIT},
    {SPR_HROD_F1, 5 * FRACUNIT},
    {SPR_HROD_F2_5, 0 * FRACUNIT},
    {SPR_HROD_F6, 4 * FRACUNIT},
    {SPR_PHNX_F1, 0 * FRACUNIT},
    {SPR_PHNX_F2, 12 * FRACUNIT},
    {SPR_PHNX_F3, 6 * FRACUNIT}
};

/*
=================
=
= R_InstallSpriteLump
=
= Local function for R_InitSprites
=================
*/

void R_InstallSpriteLump(int lump, unsigned frame, unsigned rotation,
                         boolean flipped)
{
    int r;

    if (frame >= 26 || rotation > 8)
        I_Error("R_InstallSpriteLump: Bad frame characters in lump %i", lump);

    if ((int) frame > maxframe)
        maxframe = frame;

    if (rotation == 0)
    {
// the lump should be used for all rotations
        if (sprtemp[frame].rotate == false)
            I_Error("R_InitSprites: Sprite %s frame %c has multip rot=0 lump",
                    spritename, 'A' + frame);
        if (sprtemp[frame].rotate == true)
            I_Error
                ("R_InitSprites: Sprite %s frame %c has rotations and a rot=0 lump",
                 spritename, 'A' + frame);

        sprtemp[frame].rotate = false;
        for (r = 0; r < 8; r++)
        {
            sprtemp[frame].lump[r] = lump - firstspritelump;
            sprtemp[frame].flip[r] = (byte) flipped;
        }
        return;
    }

// the lump is only used for one rotation
    if (sprtemp[frame].rotate == false)
        I_Error
            ("R_InitSprites: Sprite %s frame %c has rotations and a rot=0 lump",
             spritename, 'A' + frame);

    sprtemp[frame].rotate = true;

    rotation--;                 // make 0 based
    if (sprtemp[frame].lump[rotation] != -1)
        I_Error
            ("R_InitSprites: Sprite %s : %c : %c has two lumps mapped to it",
             spritename, 'A' + frame, '1' + rotation);

    sprtemp[frame].lump[rotation] = lump - firstspritelump;
    sprtemp[frame].flip[rotation] = (byte) flipped;
}

/*
=================
=
= R_InitSpriteDefs
=
= Pass a null terminated list of sprite names (4 chars exactly) to be used
= Builds the sprite rotation matrixes to account for horizontally flipped
= sprites.  Will report an error if the lumps are inconsistant
=Only called at startup
=
= Sprite lump names are 4 characters for the actor, a letter for the frame,
= and a number for the rotation, A sprite that is flippable will have an
= additional letter/number appended.  The rotation character can be 0 to
= signify no rotations
=================
*/

void R_InitSpriteDefs(const char **namelist)
{
    const char **check;
    int i, l, frame, rotation;
    int start, end;

// count the number of sprite names
    check = namelist;
    while (*check != NULL)
        check++;
    numsprites = check - namelist;

    if (!numsprites)
        return;

    sprites = Z_Malloc(numsprites * sizeof(*sprites), PU_STATIC, NULL);

    start = firstspritelump - 1;
    end = lastspritelump + 1;

// scan all the lump names for each of the names, noting the highest
// frame letter
// Just compare 4 characters as ints
    for (i = 0; i < numsprites; i++)
    {
        spritename = DEH_String(namelist[i]);
        memset(sprtemp, -1, sizeof(sprtemp));

        maxframe = -1;

        //
        // scan the lumps, filling in the frames for whatever is found
        //
        for (l = start + 1; l < end; l++)
            if (!strncasecmp(lumpinfo[l]->name, spritename, 4))
            {
                frame = lumpinfo[l]->name[4] - 'A';
                rotation = lumpinfo[l]->name[5] - '0';
                R_InstallSpriteLump(l, frame, rotation, false);
                if (lumpinfo[l]->name[6])
                {
                    frame = lumpinfo[l]->name[6] - 'A';
                    rotation = lumpinfo[l]->name[7] - '0';
                    R_InstallSpriteLump(l, frame, rotation, true);
                }
            }

        //
        // check the frames that were found for completeness
        //
        if (maxframe == -1)
        {
            //continue;
            sprites[i].numframes = 0;
            if (gamemode == shareware)
                continue;
            I_Error("R_InitSprites: No lumps found for sprite %s",
                    spritename);
        }

        maxframe++;
        for (frame = 0; frame < maxframe; frame++)
        {
            switch ((int) sprtemp[frame].rotate)
            {
                case -1:       // no rotations were found for that frame at all
                    I_Error("R_InitSprites: No patches found for %s frame %c",
                            spritename, frame + 'A');
                case 0:        // only the first rotation is needed
                    break;

                case 1:        // must have all 8 frames
                    for (rotation = 0; rotation < 8; rotation++)
                        if (sprtemp[frame].lump[rotation] == -1)
                            I_Error
                                ("R_InitSprites: Sprite %s frame %c is missing rotations",
                                 spritename, frame + 'A');
            }
        }

        //
        // allocate space for the frames present and copy sprtemp to it
        //
        sprites[i].numframes = maxframe;
        sprites[i].spriteframes =
            Z_Malloc(maxframe * sizeof(spriteframe_t), PU_STATIC, NULL);
        memcpy(sprites[i].spriteframes, sprtemp,
               maxframe * sizeof(spriteframe_t));
    }

}


/*
===============================================================================

							GAME FUNCTIONS

===============================================================================
*/

vissprite_t *vissprites = NULL, *vissprite_p;
int newvissprite;
static int numvissprites;


/*
===================
=
= R_InitSprites
=
= Called at program start
===================
*/

void R_InitSprites(const char **namelist)
{
    int i;

    for (i = 0; i < SCREENWIDTH; i++)
    {
        negonearray[i] = -1;
    }

    R_InitSpriteDefs(namelist);
}


/*
===================
=
= R_ClearSprites
=
= Called at frame start
===================
*/

void R_ClearSprites(void)
{
    vissprite_p = vissprites;
}


/*
===================
=
= R_NewVisSprite
=
===================
*/

vissprite_t overflowsprite;

vissprite_t *R_NewVisSprite(void)
{
    // [crispy] remove MAXVISSPRITE limit
    if (vissprite_p == &vissprites[numvissprites])
    {
	static int cap;
	int numvissprites_old = numvissprites;

	// [crispy] cap MAXVISSPRITES limit at 4096
	if (!cap && numvissprites == 32 * MAXVISSPRITES)
	{
	    fprintf(stderr, "R_NewVisSprite: MAXVISSPRITES limit capped at %d.\n", numvissprites);
	    cap++;
	}

	if (cap)
        return &overflowsprite;

	numvissprites = numvissprites ? 2 * numvissprites : MAXVISSPRITES;
	vissprites = I_Realloc(vissprites, numvissprites * sizeof(*vissprites));
	memset(vissprites + numvissprites_old, 0, (numvissprites - numvissprites_old) * sizeof(*vissprites));

	vissprite_p = vissprites + numvissprites_old;

	if (numvissprites_old)
	    fprintf(stderr, "R_NewVisSprite: Hit MAXVISSPRITES limit at %d, raised to %d.\n", numvissprites_old, numvissprites);
    }
    vissprite_p++;
    return vissprite_p - 1;
}


/*
================
=
= R_DrawMaskedColumn
=
= Used for sprites and masked mid textures
================
*/

int *mfloorclip;   // [crispy] 32-bit integer math
int *mceilingclip; // [crispy] 32-bit integer math
fixed_t spryscale;
int64_t sprtopscreen; // [crispy] WiggleFix
fixed_t sprbotscreen;

void R_DrawMaskedColumn(column_t * column, signed int baseclip)
{
    int64_t topscreen, bottomscreen; // [crispy] WiggleFix
    fixed_t basetexturemid;

    basetexturemid = dc_texturemid;
    dc_texheight = 0;

    for (; column->topdelta != 0xff;)
    {
// calculate unclipped screen coordinates for post
        topscreen = sprtopscreen + spryscale * column->topdelta;
        bottomscreen = topscreen + spryscale * column->length;
        dc_yl = (int)((topscreen + FRACUNIT - 1) >> FRACBITS); // [crispy] WiggleFix
        dc_yh = (int)((bottomscreen - 1) >> FRACBITS); // [crispy] WiggleFix

        if (dc_yh >= mfloorclip[dc_x])
            dc_yh = mfloorclip[dc_x] - 1;
        if (dc_yl <= mceilingclip[dc_x])
            dc_yl = mceilingclip[dc_x] + 1;

        if (dc_yh >= baseclip && baseclip != -1)
            dc_yh = baseclip;

        if (dc_yl <= dc_yh)
        {
            dc_source = (byte *) column + 3;
            dc_texturemid = basetexturemid - (column->topdelta << FRACBITS);
//                      dc_source = (byte *)column + 3 - column->topdelta;
            colfunc();          // either R_DrawColumn or R_DrawTLColumn
        }
        column = (column_t *) ((byte *) column + column->length + 4);
    }

    dc_texturemid = basetexturemid;
}


/*
================
=
= R_DrawVisSprite
=
= mfloorclip and mceilingclip should also be set
================
*/

void R_DrawVisSprite(vissprite_t * vis, int x1, int x2)
{
    column_t *column;
    int texturecolumn;
    fixed_t frac;
    patch_t *patch;
    fixed_t baseclip;


    patch = W_CacheLumpNum(vis->patch + firstspritelump, PU_CACHE);

    // [crispy] brightmaps for select sprites
    dc_colormap[0] = vis->colormap[0];
    dc_colormap[1] = vis->colormap[1];
    dc_brightmap = vis->brightmap;

//      if(!dc_colormap)
//              colfunc = tlcolfunc;  // NULL colormap = shadow draw

    if (vis->mobjflags & MF_SHADOW)
    {
        if (vis->mobjflags & MF_TRANSLATION)
        {
            colfunc = R_DrawTranslatedTLColumn;
            dc_translation = translationtables - 256 +
                ((vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT - 8));
        }
        else
        {                       // Draw using shadow column function
            colfunc = tlcolfunc;
        }
#ifdef CRISPY_TRUECOLOR
        blendfunc = vis->blendfunc;
#endif
    }
    else if (vis->mobjflags & MF_TRANSLATION)
    {
        // Draw using translated column function
        colfunc = R_DrawTranslatedColumn;
        dc_translation = translationtables - 256 +
            ((vis->mobjflags & MF_TRANSLATION) >> (MF_TRANSSHIFT - 8));
    }
    // [crispy] translucent sprites
    else if (crispy->translucency && vis->mobjflags & MF_TRANSLUCENT)
    {
    	if ((crispy->translucency & TRANSLUCENCY_MISSILE) ||
            (vis->psprite && crispy->translucency & TRANSLUCENCY_ITEM))
	        colfunc = tlcolfunc;
    }
    
    dc_iscale = abs(vis->xiscale) >> detailshift;
    dc_texturemid = vis->texturemid;
    frac = vis->startfrac;
    spryscale = vis->scale;

    sprtopscreen = centeryfrac - FixedMul(dc_texturemid, spryscale);

// check to see if weapon is a vissprite
    if (vis->psprite)
    {
        dc_texturemid += FixedMul(((centery - viewheight / 2) << FRACBITS),
                                  vis->xiscale);
        sprtopscreen += (viewheight / 2 - centery) << FRACBITS;
    }

    if (vis->footclip && !vis->psprite)
    {
        sprbotscreen = sprtopscreen + FixedMul(SHORT(patch->height) << FRACBITS,
                                               spryscale);
        baseclip = (sprbotscreen - FixedMul(vis->footclip << FRACBITS,
                                            spryscale)) >> FRACBITS;
    }
    else
    {
        baseclip = -1;
    }

    for (dc_x = vis->x1; dc_x <= vis->x2; dc_x++, frac += vis->xiscale)
    {
        texturecolumn = frac >> FRACBITS;
#ifdef RANGECHECK
        if (texturecolumn < 0 || texturecolumn >= SHORT(patch->width))
            I_Error("R_DrawSpriteRange: bad texturecolumn");
#endif
        column = (column_t *) ((byte *) patch +
                               LONG(patch->columnofs[texturecolumn]));
        R_DrawMaskedColumn(column, baseclip);
    }

    colfunc = basecolfunc;
#ifdef CRISPY_TRUECOLOR
    blendfunc = I_BlendOverAltTinttab;
#endif
}



/*
===================
=
= R_ProjectSprite
=
= Generates a vissprite for a thing if it might be visible
=
===================
*/

void R_ProjectSprite(mobj_t * thing)
{
    fixed_t trx, try;
    fixed_t gxt, gyt;
    fixed_t tx, tz;
    fixed_t xscale;
    int x1, x2;
    spritedef_t *sprdef;
    spriteframe_t *sprframe;
    int lump;
    unsigned rot;
    boolean flip;
    int index;
    vissprite_t *vis;
    angle_t ang;
    fixed_t iscale;

    fixed_t             interpx;
    fixed_t             interpy;
    fixed_t             interpz;
    fixed_t             interpangle;

    if (thing->flags2 & MF2_DONTDRAW)
    {                           // Never make a vissprite when MF2_DONTDRAW is flagged.
        return;
    }

    // [AM] Interpolate between current and last position,
    //      if prudent.
    if (crispy->uncapped &&
        // Don't interpolate if the mobj did something
        // that would necessitate turning it off for a tic.
        thing->interp == true &&
        // Don't interpolate during a paused state.
        leveltime > oldleveltime)
    {
        interpx = LerpFixed(thing->oldx, thing->x);
        interpy = LerpFixed(thing->oldy, thing->y);
        interpz = LerpFixed(thing->oldz, thing->z);
        interpangle = LerpAngle(thing->oldangle, thing->angle);
    }

    else
    {
        interpx = thing->x;
        interpy = thing->y;
        interpz = thing->z;
        interpangle = thing->angle;
    }

//
// transform the origin point
//
    trx = interpx - viewx;
    try = interpy - viewy;

    gxt = FixedMul(trx, viewcos);
    gyt = -FixedMul(try, viewsin);
    tz = gxt - gyt;

    if (tz < MINZ)
        return;                 // thing is behind view plane
    xscale = FixedDiv(projection, tz);

    gxt = -FixedMul(trx, viewsin);
    gyt = FixedMul(try, viewcos);
    tx = -(gyt + gxt);

    if (abs(tx) > (tz << 2))
        return;                 // too far off the side

//
// decide which patch to use for sprite reletive to player
//
#ifdef RANGECHECK
    if ((unsigned) thing->sprite >= numsprites)
        I_Error("R_ProjectSprite: invalid sprite number %i ", thing->sprite);
#endif
    sprdef = &sprites[thing->sprite];
#ifdef RANGECHECK
    if ((thing->frame & FF_FRAMEMASK) >= sprdef->numframes)
        I_Error("R_ProjectSprite: invalid sprite frame %i : %i ",
                thing->sprite, thing->frame);
#endif
    sprframe = &sprdef->spriteframes[thing->frame & FF_FRAMEMASK];

    if (sprframe->rotate)
    {                           // choose a different rotation based on player view
        ang = R_PointToAngle(interpx, interpy);
        rot = (ang - interpangle + (unsigned) (ANG45 / 2) * 9) >> 29;
        lump = sprframe->lump[rot];
        flip = (boolean) sprframe->flip[rot];
    }
    else
    {                           // use single rotation for all views
        lump = sprframe->lump[0];
        flip = (boolean) sprframe->flip[0];
    }

//
// calculate edges of the shape
//
    tx -= spriteoffset[lump];
    x1 = (centerxfrac + FixedMul(tx, xscale)) >> FRACBITS;
    if (x1 > viewwidth)
        return;                 // off the right side
    tx += spritewidth[lump];
    x2 = ((centerxfrac + FixedMul(tx, xscale)) >> FRACBITS) - 1;
    if (x2 < 0)
        return;                 // off the left side


//
// store information in a vissprite
//
    vis = R_NewVisSprite();
    vis->mobjflags = thing->flags;
    vis->psprite = false;
    vis->scale = xscale << detailshift;
    vis->gx = interpx;
    vis->gy = interpy;
    vis->gz = interpz;
    vis->gzt = interpz + spritetopoffset[lump];

    // foot clipping
    if (thing->flags2 & MF2_FEETARECLIPPED
        && interpz <= thing->subsector->sector->floorheight)
    {
        vis->footclip = 10;
    }
    else
        vis->footclip = 0;
    vis->texturemid = vis->gzt - viewz - (vis->footclip << FRACBITS);

    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth - 1 : x2;
    iscale = FixedDiv(FRACUNIT, xscale);
    if (flip)
    {
        vis->startfrac = spritewidth[lump] - 1;
        vis->xiscale = -iscale;
    }
    else
    {
        vis->startfrac = 0;
        vis->xiscale = iscale;
    }
    if (vis->x1 > x1)
        vis->startfrac += vis->xiscale * (vis->x1 - x1);
    vis->patch = lump;
//
// get light level
//

//      if (thing->flags & MF_SHADOW)
//              vis->colormap = NULL;                   // shadow draw
//      else ...

    if (fixedcolormap)
        vis->colormap[0] = vis->colormap[1] = fixedcolormap;  // fixed map
    else if (thing->frame & FF_FULLBRIGHT)
        vis->colormap[0] = vis->colormap[1] = colormaps;      // full bright
    else
    {                           // diminished light
        index = xscale >> (LIGHTSCALESHIFT - detailshift + crispy->hires);
        if (index >= MAXLIGHTSCALE)
            index = MAXLIGHTSCALE - 1;
        // [crispy] brightmaps for select sprites
        vis->colormap[0] = spritelights[index];
        vis->colormap[1] = colormaps;
    }

    vis->brightmap = R_BrightmapForSprite(thing->state - states);

#ifdef CRISPY_TRUECOLOR
    if (thing->flags & MF_SHADOW)
    {
        // [crispy] not using additive blending (I_BlendAdd) here 
        // to preserve look & feel of original Heretic's translucency
        vis->blendfunc = I_BlendOverAltTinttab;
    }
#endif
}




/*
========================
=
= R_AddSprites
=
========================
*/

void R_AddSprites(sector_t * sec)
{
    mobj_t *thing;
    int lightnum;

    if (sec->validcount == validcount)
        return;                 // already added

    sec->validcount = validcount;

    lightnum = (sec->rlightlevel >> LIGHTSEGSHIFT) + (extralight * LIGHTBRIGHT); // [crispy] smooth diminishing lighting, A11Y

    if (lightnum < 0)
        spritelights = scalelight[0];
    else if (lightnum >= LIGHTLEVELS)
        spritelights = scalelight[LIGHTLEVELS - 1];
    else
        spritelights = scalelight[lightnum];


    for (thing = sec->thinglist; thing; thing = thing->snext)
    {
        // [crispy] draw base frame and translucent current frame for MT_MUMMYLEADER attack
        if (crispy->translucency & TRANSLUCENCY_MISSILE)
        {
            if (thing->state->sprite == SPR_MUMM && !(thing->flags & MF_SHADOW) && thing->frame == (24 | FF_FULLBRIGHT))
            {
                thing->frame = (thing->frame & FF_FRAMEMASK) - 1; // [crispy] set attack stance without fire
                R_ProjectSprite(thing);
                thing->frame = 24 | FF_FULLBRIGHT; // [crispy] restore attack stance with fire
                thing->flags |= MF_TRANSLUCENT;
            }
        }
        R_ProjectSprite(thing);
        if (thing->state->sprite == SPR_MUMM && !(thing->flags & MF_SHADOW) && thing->flags & MF_TRANSLUCENT)
        {
            thing->flags &= ~MF_TRANSLUCENT;
        }
    }
}


/*
========================
=
= R_DrawPSprite
=
========================
*/

int PSpriteSY[NUMWEAPONS] = {
    0,                          // staff
    5 * FRACUNIT,               // goldwand
    15 * FRACUNIT,              // crossbow
    15 * FRACUNIT,              // blaster
    15 * FRACUNIT,              // skullrod
    15 * FRACUNIT,              // phoenix rod
    15 * FRACUNIT,              // mace
    15 * FRACUNIT,              // gauntlets
    15 * FRACUNIT               // beak
};

boolean pspr_interp = true; // [crispy]

void R_DrawPSprite(pspdef_t * psp, int psyoffset, int translucent) // [crispy] y-offset and translucency for weapon flash translucency
{
    fixed_t tx;
    int x1, x2;
    spritedef_t *sprdef;
    spriteframe_t *sprframe;
    int lump;
    boolean flip;
    vissprite_t *vis, avis;

    int tempangle;

//
// decide which patch to use
//
#ifdef RANGECHECK
    if ((unsigned) psp->state->sprite >= numsprites)
        I_Error("R_ProjectSprite: invalid sprite number %i ",
                psp->state->sprite);
#endif
    sprdef = &sprites[psp->state->sprite];
#ifdef RANGECHECK
    if ((psp->state->frame & FF_FRAMEMASK) >= sprdef->numframes)
        I_Error("R_ProjectSprite: invalid sprite frame %i : %i ",
                psp->state->sprite, psp->state->frame);
#endif
    sprframe = &sprdef->spriteframes[psp->state->frame & FF_FRAMEMASK];

    lump = sprframe->lump[0];
    flip = (boolean) sprframe->flip[0];

//
// calculate edges of the shape
//
    tx = psp->sx2 - 160 * FRACUNIT;

    tx -= spriteoffset[lump];
    if (viewangleoffset)
    {
        tempangle =
            ((centerxfrac / 1024) * (viewangleoffset >> ANGLETOFINESHIFT));
    }
    else
    {
        tempangle = 0;
    }
    x1 = (centerxfrac + FixedMul(tx, pspritescale) + tempangle) >> FRACBITS;
    if (x1 > viewwidth)
        return;                 // off the right side
    tx += spritewidth[lump];
    x2 = ((centerxfrac + FixedMul(tx, pspritescale) +
           tempangle) >> FRACBITS) - 1;
    if (x2 < 0)
        return;                 // off the left side

//
// store information in a vissprite
//
    vis = &avis;
    vis->mobjflags = 0;
    vis->psprite = true;
    vis->footclip = 0;
    // [crispy] weapons drawn 1 pixel too high when player is idle
    vis->texturemid =
        (BASEYCENTER << FRACBITS) + FRACUNIT / 4 - (psp->sy2 -
                                                    spritetopoffset[lump] + psyoffset);
    if (viewheight == SCREENHEIGHT)
    {
        vis->texturemid -= PSpriteSY[players[consoleplayer].readyweapon];
    }
    vis->x1 = x1 < 0 ? 0 : x1;
    vis->x2 = x2 >= viewwidth ? viewwidth - 1 : x2;
    vis->scale = pspritescale << detailshift;
    if (flip)
    {
        vis->xiscale = -pspriteiscale;
        vis->startfrac = spritewidth[lump] - 1;
    }
    else
    {
        vis->xiscale = pspriteiscale;
        vis->startfrac = 0;
    }
    if (vis->x1 > x1)
        vis->startfrac += vis->xiscale * (vis->x1 - x1);
    vis->patch = lump;

    if (viewplayer->powers[pw_invisibility] > 4 * 32 ||
        viewplayer->powers[pw_invisibility] & 8)
    {
        // Invisibility
        // [crispy] allow translucent weapons to be affected by invulnerability colormap
        vis->colormap[0] = vis->colormap[1] = fixedcolormap ? fixedcolormap :
                                              spritelights[MAXLIGHTSCALE - 1];
        vis->mobjflags |= MF_SHADOW;
#ifdef CRISPY_TRUECOLOR
        vis->blendfunc = I_BlendOverAltTinttab;
#endif
    }
    else if (fixedcolormap)
    {
        // Fixed color
        vis->colormap[0] = vis->colormap[1] = fixedcolormap;
    }
    else if (psp->state->frame & FF_FULLBRIGHT)
    {
        // Full bright
        vis->colormap[0] = vis->colormap[1] = colormaps;
    }
    else
    {
        // local light
        vis->colormap[0] = spritelights[MAXLIGHTSCALE - 1];
        vis->colormap[1] = colormaps;
    }
    vis->brightmap = R_BrightmapForState(psp->state - states);

    // [crispy] translucent weapon flash sprites
    if (translucent)
    {
        vis->mobjflags |= MF_TRANSLUCENT;
    }

    // [crispy] interpolate weapon bobbing
    if (crispy->uncapped)
    {
        static int     oldx1, x1_saved;
        static fixed_t oldtexturemid, texturemid_saved;
        static int     oldlump = -1;
        static int     oldgametic = -1;

        if (oldgametic < gametic)
        {
            oldx1 = x1_saved;
            oldtexturemid = texturemid_saved;
            oldgametic = gametic;
        }

        x1_saved = vis->x1;
        texturemid_saved = vis->texturemid;

        if (lump == oldlump && pspr_interp)
        {
            int deltax = vis->x2 - vis->x1;
            vis->x1 = LerpFixed(oldx1, vis->x1);
            vis->x2 = vis->x1 + deltax;
            vis->x2 = vis->x2 >= viewwidth ? viewwidth - 1 : vis->x2;
            vis->texturemid = LerpFixed(oldtexturemid, vis->texturemid);
        }
        else
        {
            oldx1 = vis->x1;
            oldtexturemid = vis->texturemid;
            oldlump = lump;
            pspr_interp = true;
        }
    }

    R_DrawVisSprite(vis, vis->x1, vis->x2);
}

/*
========================
=
= R_DrawPlayerSprites
=
========================
*/

void R_DrawPlayerSprites(void)
{
    int i, lightnum;
    int tmpframe, offset, drawbase = 0; // [crispy] for drawing base frames
    pspdef_t *psp;

//
// get light level
//
    lightnum =
        (viewplayer->mo->subsector->sector->rlightlevel >> LIGHTSEGSHIFT) + // [crispy] A11Y
        (extralight * LIGHTBRIGHT); // [crispy] smooth diminishing lighting
    if (lightnum < 0)
        spritelights = scalelight[0];
    else if (lightnum >= LIGHTLEVELS)
        spritelights = scalelight[LIGHTLEVELS - 1];
    else
        spritelights = scalelight[lightnum];
//
// clip to screen bounds
//
    mfloorclip = screenheightarray;
    mceilingclip = negonearray;

//
// add all active psprites
//
    for (i = 0, psp = viewplayer->psprites; i < NUMPSPRITES; i++, psp++)
    {
        if (psp->state)
        {
            // [crispy] draw base frame for transparent or deactivated weapon flashes
            if (!a11y_weapon_pspr ||
                    (crispy->translucency & TRANSLUCENCY_ITEM &&
                    !(viewplayer->powers[pw_invisibility] > 4*32 || viewplayer->powers[pw_invisibility] & 8)))
            {
                drawbase = 1;
                tmpframe = psp->state->frame;

                switch (psp->state->sprite)
                {         
                    case SPR_GWND:
                        if (tmpframe == 1)
                            offset = spriteoffsets[SPR_GWND_F1].offset;
                        else 
                        if (tmpframe == 2)
                            offset = spriteoffsets[SPR_GWND_F2].offset;
                        else 
                        if (tmpframe == 3)
                            offset = spriteoffsets[SPR_GWND_F3].offset;
                        else
                            drawbase = 0;
                        break;
                    case SPR_BLSR:
                        if (tmpframe == 1)
                            offset = spriteoffsets[SPR_BLSR_F1].offset;
                        else 
                        if (tmpframe == 2)
                            offset = spriteoffsets[SPR_BLSR_F2].offset;
                        else 
                        if (tmpframe == 3)
                            offset = spriteoffsets[SPR_BLSR_F3].offset;
                        else
                            drawbase = 0;
                        break;
                    case SPR_HROD:
                        if (tmpframe == 1)
                            offset = spriteoffsets[SPR_HROD_F1].offset;
                        else 
                        if (tmpframe > 1 && tmpframe < 6)
                            offset = spriteoffsets[SPR_HROD_F2_5].offset;
                        else 
                        if (tmpframe == 6)
                            offset = spriteoffsets[SPR_HROD_F6].offset;
                        else
                            drawbase = 0;
                        break;
                    case SPR_PHNX:
                        if (tmpframe == 1)
                            offset = spriteoffsets[SPR_PHNX_F1].offset;
                        else 
                        if (tmpframe == 2 || tmpframe > 3)
                            offset = spriteoffsets[SPR_PHNX_F2].offset;
                        else 
                        if (tmpframe == 3)
                            offset = spriteoffsets[SPR_PHNX_F3].offset;
                        else
                            drawbase = 0;
                        break;
                    default:
                        offset = 0x0;
                        drawbase = 0;
                        break;
                }
                if (drawbase && psp->state->sprite != SPR_GAUN)
                {
                    psp->state->frame = 0; // set base frame
                    R_DrawPSprite(psp, offset, 0);
                    psp->state->frame = tmpframe; // restore attack frame
                }
            }
            if (!a11y_weapon_pspr && drawbase) 
                continue; // [crispy] A11Y no weapon flash, use base instead
            R_DrawPSprite(psp, 0x0, drawbase); // [crispy] translucent when base was drawn
        }      
    }
}


/*
========================
=
= R_SortVisSprites
=
========================
*/

vissprite_t vsprsortedhead;

void R_SortVisSprites(void)
{
    int i, count;
    vissprite_t *ds, *best;
    static vissprite_t unsorted;
    fixed_t bestscale;

    count = vissprite_p - vissprites;

    unsorted.next = unsorted.prev = &unsorted;
    if (!count)
        return;

    for (ds = vissprites; ds < vissprite_p; ds++)
    {
        ds->next = ds + 1;
        ds->prev = ds - 1;
    }
    vissprites[0].prev = &unsorted;
    unsorted.next = &vissprites[0];
    (vissprite_p - 1)->next = &unsorted;
    unsorted.prev = vissprite_p - 1;

//
// pull the vissprites out by scale
//
    best = 0;                   // shut up the compiler warning
    vsprsortedhead.next = vsprsortedhead.prev = &vsprsortedhead;
    for (i = 0; i < count; i++)
    {
        bestscale = INT_MAX;
        for (ds = unsorted.next; ds != &unsorted; ds = ds->next)
        {
            if (ds->scale < bestscale)
            {
                bestscale = ds->scale;
                best = ds;
            }
        }
        best->next->prev = best->prev;
        best->prev->next = best->next;
        best->next = &vsprsortedhead;
        best->prev = vsprsortedhead.prev;
        vsprsortedhead.prev->next = best;
        vsprsortedhead.prev = best;
    }
}



/*
========================
=
= R_DrawSprite
=
========================
*/

void R_DrawSprite(vissprite_t * spr)
{
    drawseg_t *ds;
    int clipbot[MAXWIDTH], cliptop[MAXWIDTH]; // [crispy] 32-bit integer math
    int x, r1, r2;
    fixed_t scale, lowscale;
    int silhouette;

    for (x = spr->x1; x <= spr->x2; x++)
        clipbot[x] = cliptop[x] = -2;

//
// scan drawsegs from end to start for obscuring segs
// the first drawseg that has a greater scale is the clip seg
//
    for (ds = ds_p - 1; ds >= drawsegs; ds--)
    {
        //
        // determine if the drawseg obscures the sprite
        //
        if (ds->x1 > spr->x2 || ds->x2 < spr->x1 ||
            (!ds->silhouette && !ds->maskedtexturecol))
            continue;           // doesn't cover sprite

        r1 = ds->x1 < spr->x1 ? spr->x1 : ds->x1;
        r2 = ds->x2 > spr->x2 ? spr->x2 : ds->x2;
        if (ds->scale1 > ds->scale2)
        {
            lowscale = ds->scale2;
            scale = ds->scale1;
        }
        else
        {
            lowscale = ds->scale1;
            scale = ds->scale2;
        }

        if (scale < spr->scale || (lowscale < spr->scale
                                   && !R_PointOnSegSide(spr->gx, spr->gy,
                                                        ds->curline)))
        {
            if (ds->maskedtexturecol)   // masked mid texture
                R_RenderMaskedSegRange(ds, r1, r2);
            continue;           // seg is behind sprite
        }

//
// clip this piece of the sprite
//
        silhouette = ds->silhouette;
        if (spr->gz >= ds->bsilheight)
            silhouette &= ~SIL_BOTTOM;
        if (spr->gzt <= ds->tsilheight)
            silhouette &= ~SIL_TOP;

        if (silhouette == 1)
        {                       // bottom sil
            for (x = r1; x <= r2; x++)
                if (clipbot[x] == -2)
                    clipbot[x] = ds->sprbottomclip[x];
        }
        else if (silhouette == 2)
        {                       // top sil
            for (x = r1; x <= r2; x++)
                if (cliptop[x] == -2)
                    cliptop[x] = ds->sprtopclip[x];
        }
        else if (silhouette == 3)
        {                       // both
            for (x = r1; x <= r2; x++)
            {
                if (clipbot[x] == -2)
                    clipbot[x] = ds->sprbottomclip[x];
                if (cliptop[x] == -2)
                    cliptop[x] = ds->sprtopclip[x];
            }
        }

    }

//
// all clipping has been performed, so draw the sprite
//

// check for unclipped columns
    for (x = spr->x1; x <= spr->x2; x++)
    {
        if (clipbot[x] == -2)
            clipbot[x] = viewheight;
        if (cliptop[x] == -2)
            cliptop[x] = -1;
    }

    mfloorclip = clipbot;
    mceilingclip = cliptop;
    R_DrawVisSprite(spr, spr->x1, spr->x2);
}


/*
========================
=
= R_DrawMasked
=
========================
*/

void R_DrawMasked(void)
{
    vissprite_t *spr;
    drawseg_t *ds;

    R_SortVisSprites();

    if (vissprite_p > vissprites)
    {
        // draw all vissprites back to front

        for (spr = vsprsortedhead.next; spr != &vsprsortedhead;
             spr = spr->next)
            R_DrawSprite(spr);
    }

//
// render any remaining masked mid textures
//
    for (ds = ds_p - 1; ds >= drawsegs; ds--)
        if (ds->maskedtexturecol)
            R_RenderMaskedSegRange(ds, ds->x1, ds->x2);

//
// draw the psprites on top of everything
//
// Added for the sideviewing with an external device
    if (viewangleoffset <= 1024 << ANGLETOFINESHIFT || viewangleoffset >=
        -(1024 << ANGLETOFINESHIFT))
    {                           // don't draw on side views
        R_DrawPlayerSprites();
    }

//      if (!viewangleoffset)           // don't draw on side views
//              R_DrawPlayerSprites ();
}
