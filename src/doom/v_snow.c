//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
// Copyright(C) 2015-2021 Fabian Greffrath
// Copyright(C) 2021      Mykhailo Chernysh
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
//	[crispy] Snow drawing
//

#include <stdlib.h>
#include <stdbool.h>

#include "v_snow.h"
#include "i_video.h"
#include "z_zone.h"
#include "r_main.h"

typedef struct snowflake_t
{
    int x, y;
} snowflake_t;

static snowflake_t *snowflakes = NULL;
static size_t snowflakes_num;
static int snowflakes_color = -1;
static int last_screen_size;
static int wind = 1;

static void InitSnowCoords()
{
    for (size_t i = 0; i < snowflakes_num; i++)
    {
        snowflakes[i].y = 0 - rand() % (SCREENHEIGHT * 2);
        snowflakes[i].x = rand() % (SCREENWIDTH);
    }
}

void V_SnowUpdate()
{
    if (rand() % 150 == 1)
        wind = 1 - rand() % 3;

    for (size_t i = 0; i < snowflakes_num; i++)
    {
        snowflakes[i].y += rand() % 2;
        if (snowflakes[i].y < 0)
            continue;

        snowflakes[i].x += 1 - rand() % 3;
        snowflakes[i].x += wind;

        if (snowflakes[i].y >= SCREENHEIGHT)
            snowflakes[i].y = 0;
        if (snowflakes[i].x >= SCREENWIDTH)
            snowflakes[i].x = snowflakes[i].x - SCREENWIDTH;
        if (snowflakes[i].x < 0)
            snowflakes[i].x = SCREENWIDTH - snowflakes[i].x;
    }
}

static void ResetSnow()
{
    if (snowflakes != NULL)
        Z_Free(snowflakes);

    last_screen_size = SCREENWIDTH * SCREENHEIGHT;
    snowflakes_num = last_screen_size >> 6;
    snowflakes = Z_Malloc(snowflakes_num * sizeof(snowflake_t), PU_STATIC, NULL);

    InitSnowCoords();

    if (snowflakes_color == -1)
        snowflakes_color = I_GetPaletteIndex(0xFF, 0xFF, 0xFF);
}

void V_SnowDraw()
{
    int video_offset;

    if (last_screen_size != (SCREENHEIGHT * SCREENWIDTH))
        ResetSnow();

    for (size_t i = 0; i < snowflakes_num; i++)
    {
        if (snowflakes[i].y < 0)
            continue;

        video_offset = snowflakes[i].x + snowflakes[i].y * SCREENWIDTH;
        I_VideoBuffer[video_offset] = snowflakes_color;
    }
}
