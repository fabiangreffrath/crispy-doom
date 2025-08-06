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

#ifndef __DDMAPINFO_H__
#define __DDMAPINFO_H__

typedef struct 
{
	int ofs_lump;
	int ofs_displayed_name;
	int ofs_next;
	int ofs_secret_next;
	int par;
	int ofs_music;
	int episode_number;
	int map_number;
	int ofs_endsequence;
	int ofs_sky1;
	int sky_scroll_speed;
	int map07special;
}
dmapinfo_map_t;

typedef struct 
{
	int ofs_first_level_lump;
	int ofs_episode_name;
}
dmapinfo_episode_t;

typedef struct
{
	int ofs_id;
	int ofs_text;
	int ofs_flat;
	int ofs_music;
}
dmapinfo_endsequence_t;

typedef struct
{
	dmapinfo_map_t *maps;
	int num_maps;

	dmapinfo_episode_t *episodes;
	int num_episodes;

	dmapinfo_endsequence_t *endsequences;
	int num_endsequences;

	char *strings;
	int strings_len;
	int strings_maxlen;
}
dmapinfo_t;

extern dmapinfo_t dmapinfo;

int DMAPINFO_LoadLump(int lumpnum);
const char *DMAPINFO_GetString(int offset);
int DMAPINFO_GetLevelMusic();
void DMAPINFO_GetFinale(const char **text, const char **flat, int *music);
dmapinfo_map_t *DMAPINFO_GetMap(int episode, int map);
dmapinfo_map_t *DMAPINFO_GetMapByLumpname(const char *lumpname);

#endif
