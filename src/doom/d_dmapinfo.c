//
// Copyright(C) 2021 James Canete
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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "i_system.h" // I_Realloc()
#include "w_wad.h"
#include "z_zone.h"

#include "doomstat.h"
#include "d_mode.h"
#include "p_setup.h"

#include "d_dmapinfo.h"

dmapinfo_t dmapinfo;

static int dmapinfo_initialized = 0;


static const char *skip_whitespace(const char *in, const char *in_end)
{
    for (; in < in_end; in++)
    {
        if (!isspace(*in))
            break;
    }

    return in;
}

static const char *skip_quote(const char *in, const char *in_end)
{
    if (*in != '"')
        return in;

    for (in++; in < in_end; in++)
    {
        if (*in == '"')
            return in + 1;

        if (*in == '\\' && in + 1 < in_end)
            in++;
    }

    return in;
}

static const char *skip_block(const char *in, const char *in_end)
{
    int depth = 0;

    for (; in < in_end; in++)
    {
        if (*in == '{')
            depth++;
        else if (*in == '}')
            depth--;

        if (depth == 0)
            return in + 1;

        if (*in == '"')
            in = skip_quote(in, in_end) - 1;
    }

    return in;
}

static const char *skip_token(const char *in, const char *in_end)
{
    if (*in == '"')
        return skip_quote(in, in_end);

    if (*in == '{')
        return skip_block(in, in_end);

    for (; in < in_end; in++)
    {
        if (isspace(*in))
            break;
    }

    return in;
}

static int is_comment(const char *in, const char *in_end)
{
    if (in + 1 < in_end && *in == '/' && in[1] == '/')
        return 1;
    return 0;
}

static const char *skip_comment(const char *in, const char *in_end)
{
    if (is_comment(in, in_end))
    {
        in += 2;
        while (in < in_end && *in != '\n')
            in++;
    }

    return in;
}

static const char *first_token(const char *in, const char *in_end)
{
    in = skip_whitespace(in, in_end);
    while (is_comment(in, in_end))
    {
        in = skip_comment(in, in_end);
        in = skip_whitespace(in, in_end);
    }

    return in;
}

static const char *next_token(const char *in, const char *in_end)
{
    in = skip_token(in, in_end);
    return first_token(in, in_end);
}

static int escape_string(const char *in, const char *in_end, char *out, char *out_end)
{
    int len = 0;

    in_end = skip_token(in, in_end);

    if (in >= in_end)
        return 0;

    if (*in != '"')
    {
        if (out)
        {
            for (; in < in_end && out < out_end; len++)
                *out++ = *in++;
        }

        for (; in < in_end; len++)
            in++;

        if (out)
        {
            if (out >= out_end)
                out = out_end - 1;

            *out++ = '\0';
        }
        len++;

        return len;
    }

    in++;

    if (in_end > in && in_end[-1] == '"')
        in_end--;

    if (out)
    {
        for (; in < in_end && out < out_end; len++)
        {
            char c;

            if (*in == '\\')
            {
                in++;
                if (in >= in_end)
                    break;

                switch(*in++)
                {
                    case 'a':
                        c = '\a'; break;
                    case 'b':
                        c = '\b'; break;
                    case 'e':
                        c = '\e'; break;
                    case 'f':
                        c = '\f'; break;
                    case 'n':
                        c = '\n'; break;
                    case 'r':
                        c = '\r'; break;
                    case 't':
                        c = '\t'; break;
                    case 'v':
                        c = '\v'; break;
                    case '\\':
                        c = '\\'; break;
                    case '\'':
                        c = '\''; break;
                    case '"':
                        c = '\"'; break;
                    case '0':
                        c = '\0'; break;
                    default:
                        c = '?'; break;
                }
            }
            else
            {
                c = *in++;
            }

            *out++ = c;
        }
    }

    for (; in < in_end; len++)
    {
        if (*in == '\\')
        {
            in++;
            if (in >= in_end)
                break;
        }
        in++;
    }

    if (out)
    {
        if (out >= out_end)
            out = out_end - 1;

        *out++ = '\0';
    }
    len++;

    return len;
}

static int dmapinfo_add_escape_string(const char *in, const char *in_end)
{
    int p = dmapinfo.strings_len;
    int l = escape_string(in, in_end, NULL, NULL);
    int s = dmapinfo.strings_maxlen;

    while (p + l >= s)
    {
        if (!s)
            s = 1024;
        else
            s *= 2;
    }

    if (dmapinfo.strings_maxlen != s)
    {
        dmapinfo.strings = I_Realloc(dmapinfo.strings, s);
        dmapinfo.strings_maxlen = s;
    }

    escape_string(in, in_end, dmapinfo.strings + p, dmapinfo.strings + p + l);
    dmapinfo.strings_len += l;

    return p;
}

static int range_icmp(const char *a, const char *a_end, const char *b, const char *b_end)
{
    for (;a < a_end && b < b_end; a++, b++)
    {
        int r = tolower(*(unsigned char *)a) - tolower(*(unsigned char *)b);
        if (r)
            return r;
    }

    if (a != a_end)
        return tolower(*(unsigned char *)a);

    if (b != b_end)
        return -tolower(*(unsigned char *)b);

    return 0;
}

static int token_stricmp(const char *s, const char *t, const char *t_end)
{
    t_end = skip_token(t, t_end);
    return range_icmp(s, s + strlen(s), t, t_end);
}

static int dmapinfo_add_string_from_field(int *out, const char *field, const char **in, const char *in_end)
{
    if (token_stricmp(field, *in, in_end) != 0)
        return 0;

    *in = next_token(*in, in_end);
    *in = next_token(*in, in_end);

    *out = dmapinfo_add_escape_string(*in, in_end);
    *in = next_token(*in, in_end);

    return 1;
}

static int get_int_from_token(const char *in, const char *in_end)
{
    char s[256];
    int l;

    in_end = skip_token(in, in_end);

    if (*in == '"')
    {
        in++;

        if (in < in_end && in_end[-1] == '"')
            in_end--;
    }

    l = in_end - in;
    if (l > 255)
        l = 255;
    
    strncpy(s, in, l);
    s[l] = '\0';

    return atoi(s);
}

static int dmapinfo_get_int_from_field(int *out, const char *field, const char **in, const char *in_end)
{
    if (token_stricmp(field, *in, in_end) != 0)
        return 0;

    *in = next_token(*in, in_end);
    *in = next_token(*in, in_end);

    if (*in < in_end)
        *out = get_int_from_token(*in, in_end);

    *in = next_token(*in, in_end);

    return 1;
}

static int dmapinfo_get_bool_from_field(int *out, const char *field, const char **in, const char *in_end)
{
    if (token_stricmp(field, *in, in_end) != 0)
        return 0;

    *in = next_token(*in, in_end);

    *out = 1;
    return 1;
}

int DMAPINFO_Init()
{
    dmapinfo_initialized = 1;
    memset(&dmapinfo, 0, sizeof(dmapinfo));
    return 1;
}

int DMAPINFO_LoadLump(int lumpnum)
{
    const char *lump, *token;
    int size, i;

    dmapinfo_map_t *map;
    dmapinfo_episode_t *episode;
    dmapinfo_endsequence_t *endseq;

    int num_maps, num_episodes, num_endseqs;

    if (!dmapinfo_initialized)
        DMAPINFO_Init();

    lump = W_CacheLumpNum(lumpnum, PU_STATIC);
    size = W_LumpLength(lumpnum);

    num_maps = 0;
    num_episodes = 0;
    num_endseqs = 0;

    token = first_token(lump, lump + size);
    while (token < lump + size)
    {
        if (token_stricmp("map", token, lump + size) == 0)
            num_maps++;
        else if (token_stricmp("episode", token, lump + size) == 0)
            num_episodes++;
        else if (token_stricmp("endsequence", token, lump + size) == 0)
            num_endseqs++;
        token = next_token(token, lump + size);
    }
    
    //printf("%d maps\n", dmapinfo.num_maps);
    //printf("%d episodes\n", dmapinfo.num_episodes);
    //printf("%d endsequences\n", dmapinfo.num_endsequences);

    dmapinfo.maps         = I_Realloc(dmapinfo.maps,         sizeof(*dmapinfo.maps)         * (dmapinfo.num_maps + num_maps));
    dmapinfo.episodes     = I_Realloc(dmapinfo.episodes,     sizeof(*dmapinfo.episodes)     * (dmapinfo.num_episodes + num_episodes));
    dmapinfo.endsequences = I_Realloc(dmapinfo.endsequences, sizeof(*dmapinfo.endsequences) * (dmapinfo.num_endsequences + num_endseqs));

    map = dmapinfo.maps + dmapinfo.num_maps;
    for (i = 0; i < num_maps; i++, map++)
    {
        map->ofs_lump = -1;
        map->ofs_displayed_name = -1;
        map->ofs_next = -1;
        map->ofs_secret_next = -1;
        map->par = 0;
        map->ofs_music = -1;
        map->episode_number = 0;
        map->map_number = 0;
        map->ofs_endsequence = -1;
        map->ofs_sky1 = -1;
        map->sky_scroll_speed = 0;
        map->map07special = 0;
    }

    episode = dmapinfo.episodes + dmapinfo.num_episodes;
    for (i = 0; i < num_episodes; i++, episode++)
    {
        episode->ofs_first_level_lump = -1;
        episode->ofs_episode_name = -1;
    }

    endseq = dmapinfo.endsequences + dmapinfo.num_endsequences;
    for (i = 0; i < num_endseqs; i++, endseq++)
    {
        endseq->ofs_id = -1;
        endseq->ofs_text = -1;
        endseq->ofs_flat = -1;
        endseq->ofs_music = -1;
    }

    map =     dmapinfo.maps         + dmapinfo.num_maps;
    episode = dmapinfo.episodes     + dmapinfo.num_episodes;
    endseq =  dmapinfo.endsequences + dmapinfo.num_endsequences;

    token = first_token(lump, lump + size);
    while (token < lump + size)
    {
        if (token_stricmp("map", token, lump + size) == 0)
        {
            token = next_token(token, lump + size);
            map->ofs_lump = dmapinfo_add_escape_string(token, lump + size);

            token = next_token(token, lump + size);
            map->ofs_displayed_name = dmapinfo_add_escape_string(token, lump + size);

            token = next_token(token, lump + size);
            if (token < lump + size && *token == '{')
            {
                const char *block = token;
                const char *block_end = skip_token(token, lump + size);
                const char *token;

                block++;
                if (block < block_end && block_end[-1] == '}')
                    block_end--;

                token = first_token(block, block_end);
                while (token < block_end)
                {
                    if (dmapinfo_add_string_from_field(&map->ofs_next,        "next",          &token, block_end)) continue;
                    if (dmapinfo_add_string_from_field(&map->ofs_secret_next, "secretnext",    &token, block_end)) continue;
                    if (dmapinfo_get_int_from_field(   &map->par,             "par",           &token, block_end)) continue;
                    if (dmapinfo_add_string_from_field(&map->ofs_music,       "music",         &token, block_end)) continue;
                    if (dmapinfo_get_int_from_field(   &map->episode_number,  "episodenumber", &token, block_end))
                    {
                        // hack to allow multiple episodes from multiple pwads
                        // not that useful, since levels with same named lumps still override each other
                        map->episode_number += dmapinfo.num_episodes;
                        continue;
                    }
                    if (dmapinfo_get_int_from_field(   &map->map_number,      "mapnumber",     &token, block_end)) continue;
                    if (dmapinfo_add_string_from_field(&map->ofs_endsequence, "endsequence",   &token, block_end)) continue;

                    if (dmapinfo_add_string_from_field(&map->ofs_sky1,        "sky1",          &token, block_end))
                    {
                        if (token < block_end && *token == ',')
                        {
                            token = next_token(token, block_end);
                        
                            map->sky_scroll_speed = get_int_from_token(token, block_end);
                            token = next_token(token, block_end);
                        }
                        continue;
                    }

                    if (dmapinfo_get_bool_from_field  (&map->map07special,   "map07special",  &token, block_end)) continue;

                    printf("unknown token \"%.*s\"\n", (int)(skip_token(token, block_end) - token), token);
                    token = next_token(token, block_end);
                }
            }
            map++;
        }
        else if (token_stricmp("episode", token, lump + size) == 0)
        {
            token = next_token(token, lump + size);
            episode->ofs_first_level_lump = dmapinfo_add_escape_string(token, lump + size);

            token = next_token(token, lump + size);
            if (token < lump + size && *token == '{')
            {
                const char *block = token;
                const char *block_end = skip_token(token, lump + size);
                const char *token;

                block++;
                if (block < block_end && block_end[-1] == '}')
                    block_end--;

                token = first_token(block, block_end);
                while (token < block_end)
                {
                    if (dmapinfo_add_string_from_field(&episode->ofs_episode_name, "name", &token, block_end)) continue;

                    printf("unknown token \"%.*s\"\n", (int)(skip_token(token, block_end) - token), token);
                    token = next_token(token, block_end);
                }
            }
            episode++;
        }
        else if (token_stricmp("endsequence", token, lump + size) == 0)
        {
            token = next_token(token, lump + size);
            endseq->ofs_id = dmapinfo_add_escape_string(token, lump + size);

            token = next_token(token, lump + size);
            if (token < lump + size && *token == '{')
            {
                const char *block = token;
                const char *block_end = skip_token(token, lump + size);
                const char *token;

                block++;
                if (block < block_end && block_end[-1] == '}')
                    block_end--;

                token = first_token(block, block_end);
                while (token < block_end)
                {
                    if (dmapinfo_add_string_from_field(&endseq->ofs_text, "text", &token, block_end))
                    {
                        // special casing to extend text
                        while (token < block_end && *token == '"')
                        {
                            dmapinfo.strings[dmapinfo.strings_len - 1] = '\n';
                            dmapinfo_add_escape_string(token, block_end);
                            token = next_token(token, block_end);
                        }

                        continue;
                    }
                    if (dmapinfo_add_string_from_field(&endseq->ofs_flat,  "flat",  &token, block_end)) continue;
                    if (dmapinfo_add_string_from_field(&endseq->ofs_music, "music", &token, block_end)) continue;

                    printf("unknown token \"%.*s\"\n", (int)(skip_token(token, block_end) - token), token);
                    token = next_token(token, block_end);
                }
            }
            endseq++;
        }

        token = next_token(token, lump + size);
    }

    dmapinfo.num_maps         += num_maps;
    dmapinfo.num_episodes     += num_episodes;
    dmapinfo.num_endsequences += num_endseqs;

    return 1;
}

const char *DMAPINFO_GetString(int offset)
{
    if (offset == -1)
        return NULL;
    return dmapinfo.strings + offset;
}

int DMAPINFO_GetLevelMusic(int *lump)
{
    dmapinfo_map_t *d_map = DMAPINFO_GetMap(gameepisode, gamemap);
    const char *lumpname;

    if (!d_map)
        return -1;

    lumpname = DMAPINFO_GetString(d_map->ofs_music);
    if (!lumpname)
        return -1;

    return W_CheckNumForName(lumpname);
}

void DMAPINFO_GetFinale(const char **text, const char **flat, int *music)
{
    dmapinfo_map_t *map;
    const char *endseq_name;
    dmapinfo_endsequence_t *endseq;
    int i;

    if (!dmapinfo.num_endsequences)
        return;

    map = DMAPINFO_GetMap(gameepisode, gamemap);
    if (!map)
        return;

    endseq_name = DMAPINFO_GetString(map->ofs_endsequence);

    if (!endseq_name)
        return;

    endseq = dmapinfo.endsequences;
    for (i = 0; i < dmapinfo.num_endsequences; i++, endseq++)
    {
        if (strcmp(DMAPINFO_GetString(endseq->ofs_id), endseq_name) == 0)
            break;
    }

    if (i == dmapinfo.num_endsequences)
        return;

    *music = W_CheckNumForName(DMAPINFO_GetString(endseq->ofs_music));
    *flat = DMAPINFO_GetString(endseq->ofs_flat);
    *text = DMAPINFO_GetString(endseq->ofs_text);
}

dmapinfo_map_t *DMAPINFO_GetMap(int episode, int map)
{
    dmapinfo_map_t *d_map = dmapinfo.maps;
    int i;

    for (i = 0; i < dmapinfo.num_maps; i++, d_map++)
    {
        if (d_map->episode_number == episode && d_map->map_number == map)
        {
            break;
        }
    }

    if (i == dmapinfo.num_maps)
    {
        return NULL;
    }

    return d_map;
}

dmapinfo_map_t *DMAPINFO_GetMapByLumpname(const char *lumpname)
{
    dmapinfo_map_t *d_map = dmapinfo.maps;
    int i;

    for (i = 0; i < dmapinfo.num_maps; i++, d_map++)
    {
        if (strcmp(DMAPINFO_GetString(d_map->ofs_lump), lumpname) == 0)
        {
            break;
        }
    }

    if (i == dmapinfo.num_maps)
        return NULL;

    return d_map;
}
