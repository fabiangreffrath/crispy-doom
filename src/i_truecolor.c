//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2015-2024 Fabian Greffrath
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
//	[crispy] Truecolor rendering
//

#include "config.h"

#ifdef CRISPY_TRUECOLOR

#include "crispy.h"
#include "i_truecolor.h"

const uint32_t (*blendfunc) (const uint32_t fg, const uint32_t bg) = I_BlendOverTranmap;

typedef union
{
    uint32_t i;
    struct {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t a;
    };
} tcpixel_t;

// [JN] LUTs to store precomputed values for all possible 512 color combinations
static uint32_t blendAddLUT[512][512];      // Additive blending
static uint32_t blendOverLUT[512][512];     // Overlay blending
static uint32_t blendOverAltLUT[512][512];  // Overlay "alt" blending

const uint32_t (*I_BlendAddFunc) (const uint32_t bg_i, const uint32_t fg_i);
const uint32_t (*I_BlendOverFunc) (const uint32_t bg_i, const uint32_t fg_i, const int amount);
//const uint32_t (*I_BlendOverAltFunc) (const uint32_t bg_i, const uint32_t fg_i);

// [JN] Different blending alpha values for different games
#define OVERLAY_ALPHA_TRANMAP     168  // Doom: TRANMAP, 66% opacity
#define OVERLAY_ALPHA_TINTTAB     96   // Raven: TINTTAB, 38% opacity
#define OVERLAY_ALPHA_ALTTINTTAB  142  // Raven: TINTTAB "Alt", 56% opacity
#define OVERLAY_ALPHA_XLATAB      192  // Strife: XLATAB, 75% opacity
#define OVERLAY_ALPHA_ALTXLATAB   64   // Strife: XLATAB "Alt", 25% opacity


// [JN] Initialize blending maps for tablified additive and overlay translucency.
// TODO: Heretic, Hexen and Strife support, will need "Alt" blendOverLUT, and
// will NOT need blendAddLUT at all.
void R_InitBlendMaps (void)
{
    tcpixel_t bg, fg;
    tcpixel_t retAdd, retOver;
    uint8_t overlay_alpha;

    // Shortcut: these variables are always same in tablified approach
    retAdd.a = 0xFFU;
    retOver.a = 0xFFU;

    // TODO: so here we can define different alpha value for different games
    overlay_alpha = OVERLAY_ALPHA_TRANMAP;

    for (int bg_index = 0; bg_index < 512; ++bg_index)
    {
        for (int fg_index = 0; fg_index < 512; ++fg_index)
        {
            // Convert LUT indices back to RGB values with reduced bit depth
            bg.r = (bg_index >> 6) << 5;
            bg.g = ((bg_index >> 3) & 0x07) << 5;
            bg.b = (bg_index & 0x07) << 5;

            fg.r = (fg_index >> 6) << 5;
            fg.g = ((fg_index >> 3) & 0x07) << 5;
            fg.b = (fg_index & 0x07) << 5;

            // Additive blending calculation
            retAdd.r = MIN(bg.r + fg.r, 0xFFU);
            retAdd.g = MIN(bg.g + fg.g, 0xFFU);
            retAdd.b = MIN(bg.b + fg.b, 0xFFU);
            blendAddLUT[bg_index][fg_index] = retAdd.i;

            // Overlay blending calculation
            retOver.r = (overlay_alpha * fg.r + (0xFFU - overlay_alpha) * bg.r) >> 8;
            retOver.g = (overlay_alpha * fg.g + (0xFFU - overlay_alpha) * bg.g) >> 8;
            retOver.b = (overlay_alpha * fg.b + (0xFFU - overlay_alpha) * bg.b) >> 8;
            blendOverLUT[bg_index][fg_index] = retOver.i;
            blendOverAltLUT[bg_index][fg_index] = retAdd.i;
        }
    }
}

// [JN] Helper function to convert a pixel color to a LUT index
static inline uint16_t PixelToLUTIndex (const tcpixel_t color)
{
    return ((color.r & 0xE0) << 1) | ((color.g & 0xE0) >> 2) | (color.b >> 5);
}

const uint32_t I_BlendAdd (const uint32_t bg_i, const uint32_t fg_i)
{
    tcpixel_t bg, fg, ret;

    bg.i = bg_i;
    fg.i = fg_i;

    ret.a = 0xFFU;
    ret.r = MIN(bg.r + fg.r, 0xFFU);
    ret.g = MIN(bg.g + fg.g, 0xFFU);
    ret.b = MIN(bg.b + fg.b, 0xFFU);

    return ret.i;
}

const uint32_t I_BlendAddLow (const uint32_t bg_i, const uint32_t fg_i)
{
    tcpixel_t bg, fg;
    uint16_t bg_index, fg_index;

    bg.i = bg_i;
    fg.i = fg_i;

    bg_index = PixelToLUTIndex(bg);
    fg_index = PixelToLUTIndex(fg);

    return blendAddLUT[bg_index][fg_index];
}

const uint32_t I_BlendDark (const uint32_t bg_i, const int d)
{
    tcpixel_t bg, ret;

    bg.i = bg_i;

    ret.a = 0xFFU;
    ret.r = (bg.r * d) >> 8;
    ret.g = (bg.g * d) >> 8;
    ret.b = (bg.b * d) >> 8;

    return ret.i;
}

const uint32_t I_BlendOver (const uint32_t bg_i, const uint32_t fg_i, const int amount)
{
    tcpixel_t bg, fg, ret;

    bg.i = bg_i;
    fg.i = fg_i;

    ret.a = 0xFFU;
    ret.r = (amount * fg.r + (0XFFU - amount) * bg.r) >> 8;
    ret.g = (amount * fg.g + (0XFFU - amount) * bg.g) >> 8;
    ret.b = (amount * fg.b + (0XFFU - amount) * bg.b) >> 8;

    return ret.i;
}

const uint32_t I_BlendOverLow (const uint32_t bg_i, const uint32_t fg_i, const int amount)
{
    tcpixel_t bg, fg;
    uint16_t bg_index, fg_index;

    bg.i = bg_i;
    fg.i = fg_i;

    bg_index = PixelToLUTIndex(bg);
    fg_index = PixelToLUTIndex(fg);

    return blendOverLUT[bg_index][fg_index];
}

// [crispy] TRANMAP blending emulation, used for Doom
const uint32_t I_BlendOverTranmap (const uint32_t bg, const uint32_t fg)
{
    return I_BlendOverFunc(bg, fg, 0xA8); // 168 (66% opacity)
}

// [crispy] TINTTAB blending emulation, used for Heretic and Hexen
const uint32_t I_BlendOverTinttab (const uint32_t bg, const uint32_t fg)
{
    return I_BlendOver(bg, fg, 0x60); // 96 (38% opacity)
}

// [crispy] More opaque ("Alt") TINTTAB blending emulation, used for Hexen's MF_ALTSHADOW drawing
const uint32_t I_BlendOverAltTinttab (const uint32_t bg, const uint32_t fg)
{
    return I_BlendOver(bg, fg, 0x8E); // 142 (56% opacity)
}

// [crispy] More opaque XLATAB blending emulation, used for Strife
const uint32_t I_BlendOverXlatab (const uint32_t bg, const uint32_t fg)
{
    return I_BlendOver(bg, fg, 0xC0); // 192 (75% opacity)
}

// [crispy] Less opaque ("Alt") XLATAB blending emulation, used for Strife
const uint32_t I_BlendOverAltXlatab (const uint32_t bg, const uint32_t fg)
{
    return I_BlendOver(bg, fg, 0x40); // 64 (25% opacity)
}

// [JN] Set pointers to blending functions.
void R_InitBlendQuality (void)
{
    if (crispy->blendquality)
    {
        I_BlendAddFunc = I_BlendAdd;
        I_BlendOverFunc = I_BlendOver;
        // I_BlendOverAltFunc = I_BlendOverAlt;
    }
    else
    {
        I_BlendAddFunc = I_BlendAddLow;
        I_BlendOverFunc = I_BlendOverLow;
        // I_BlendOverAltFunc = I_BlendOverAltLow;
    }
}

#endif
