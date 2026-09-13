//
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
// DESCRIPTION:
//	System interface for PC speaker sound.
//

#include "SDL.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "crispy.h"
#include "doomtype.h"

#include "deh_str.h"
#include "i_sound.h"
#include "memio.h"
#include "m_misc.h"
#include "midifile.h"
#include "mus2mid.h"
#include "w_wad.h"
#include "z_zone.h"

#include "pcsound.h"

#define TIMER_FREQ 1193181 /* hz */

static boolean pcs_backend_initialized = false;
static boolean pcs_sound_initialized = false;
static boolean pcs_music_initialized = false;

static SDL_mutex *sound_lock;
static GameMission_t gamemission;

static uint8_t *current_sound_lump = NULL;
static uint8_t *current_sound_pos = NULL;
static unsigned int current_sound_remaining = 0;
static int current_sound_handle = 0;
static int current_sound_lump_num = -1;

#define PCSOUND_MUSIC_TICK_US 7000
#define PCSOUND_MUSIC_DEFAULT_TEMPO 500000
#define PCSOUND_MUSIC_CHANNELS 16
#define PCSOUND_MUSIC_NOTES 128

typedef struct
{
    midi_track_iter_t *iter;
    uint64_t delay_us;
    boolean finished;
} pcs_music_track_t;

static midi_file_t *current_music_file;
static pcs_music_track_t *music_tracks;
static unsigned int music_num_tracks;
static unsigned int music_ticks_per_beat;
static unsigned int music_tempo;
static boolean music_playing;
static boolean music_looping;
static boolean music_paused;
static int music_volume = 127;
static int music_frequency;
static uint64_t music_note_order[PCSOUND_MUSIC_CHANNELS][PCSOUND_MUSIC_NOTES];
static uint64_t music_next_note_order;

static const uint16_t divisors[] = {
    0,
    6818, 6628, 6449, 6279, 6087, 5906, 5736, 5575,
    5423, 5279, 5120, 4971, 4830, 4697, 4554, 4435,
    4307, 4186, 4058, 3950, 3836, 3728, 3615, 3519,
    3418, 3323, 3224, 3131, 3043, 2960, 2875, 2794,
    2711, 2633, 2560, 2485, 2415, 2348, 2281, 2213,
    2153, 2089, 2032, 1975, 1918, 1864, 1810, 1757,
    1709, 1659, 1612, 1565, 1521, 1478, 1435, 1395,
    1355, 1316, 1280, 1242, 1207, 1173, 1140, 1107,
    1075, 1045, 1015,  986,  959,  931,  905,  879,
     854,  829,  806,  783,  760,  739,  718,  697,
     677,  658,  640,  621,  604,  586,  570,  553,
     538,  522,  507,  493,  479,  465,  452,  439,
     427,  415,  403,  391,  380,  369,  359,  348,
     339,  329,  319,  310,  302,  293,  285,  276,
     269,  261,  253,  246,  239,  232,  226,  219,
     213,  207,  201,  195,  190,  184,  179,
};

static int PCSMusicNoteFrequency(unsigned int note)
{
    return (int) (440.0 * pow(2.0, ((double) note - 69.0) / 12.0) + 0.5);
}

static int PCSMusicCurrentFrequency(void)
{
    uint64_t newest_note;
    unsigned int channel;
    unsigned int note;
    unsigned int current_note;

    newest_note = 0;
    current_note = 0;

    // The PC speaker is monophonic; use the most recently started note.
    for (channel = 0; channel < PCSOUND_MUSIC_CHANNELS; ++channel)
    {
        for (note = 0; note < PCSOUND_MUSIC_NOTES; ++note)
        {
            if (music_note_order[channel][note] > newest_note)
            {
                newest_note = music_note_order[channel][note];
                current_note = note;
            }
        }
    }

    if (newest_note == 0)
    {
        return 0;
    }

    return PCSMusicNoteFrequency(current_note);
}

static uint64_t PCSMusicTicksToMicroseconds(unsigned int ticks)
{
    if (music_ticks_per_beat == 0)
    {
        return 0;
    }

    return ((uint64_t) ticks * music_tempo) / music_ticks_per_beat;
}

static void PCSMusicClearNotes(void)
{
    memset(music_note_order, 0, sizeof(music_note_order));
    music_next_note_order = 0;
    music_frequency = 0;
}

static void PCSMusicFreeTracks(void)
{
    unsigned int i;

    if (music_tracks == NULL)
    {
        return;
    }

    for (i = 0; i < music_num_tracks; ++i)
    {
        MIDI_FreeIterator(music_tracks[i].iter);
    }

    free(music_tracks);
    music_tracks = NULL;
    music_num_tracks = 0;
}

static void PCSMusicStop(void)
{
    PCSMusicFreeTracks();
    music_playing = false;
    music_paused = false;
    PCSMusicClearNotes();
}

static void PCSMusicProcessEvent(midi_event_t *event)
{
    unsigned int channel;
    unsigned int note;

    switch (event->event_type)
    {
        case MIDI_EVENT_NOTE_OFF:
        case MIDI_EVENT_NOTE_ON:
            channel = event->data.channel.channel;
            note = event->data.channel.param1;

            if (channel >= PCSOUND_MUSIC_CHANNELS
             || note >= PCSOUND_MUSIC_NOTES)
            {
                break;
            }

            if (event->event_type == MIDI_EVENT_NOTE_OFF
             || event->data.channel.param2 == 0)
            {
                music_note_order[channel][note] = 0;
            }
            else
            {
                ++music_next_note_order;
                music_note_order[channel][note] = music_next_note_order;
            }
            break;

        case MIDI_EVENT_CONTROLLER:
            channel = event->data.channel.channel;

            if (channel >= PCSOUND_MUSIC_CHANNELS)
            {
                break;
            }

            if (event->data.channel.param1 == MIDI_CONTROLLER_ALL_SOUND_OFF
             || event->data.channel.param1 == MIDI_CONTROLLER_RESET_ALL_CTRLS
             || event->data.channel.param1 == MIDI_CONTROLLER_ALL_NOTES_OFF)
            {
                for (note = 0; note < PCSOUND_MUSIC_NOTES; ++note)
                {
                    music_note_order[channel][note] = 0;
                }
            }
            break;

        case MIDI_EVENT_META:
            if (event->data.meta.type == MIDI_META_SET_TEMPO
             && event->data.meta.length == 3)
            {
                music_tempo = (event->data.meta.data[0] << 16)
                            | (event->data.meta.data[1] << 8)
                            | event->data.meta.data[2];
            }
            break;

        default:
            break;
    }
}

static void PCSMusicStartTracks(void)
{
    unsigned int i;

    for (i = 0; i < music_num_tracks; ++i)
    {
        MIDI_RestartIterator(music_tracks[i].iter);
        music_tracks[i].delay_us = PCSMusicTicksToMicroseconds(
            MIDI_GetDeltaTime(music_tracks[i].iter));
        music_tracks[i].finished = false;
    }

    PCSMusicClearNotes();
}

static void PCSMusicAdvance(void)
{
    unsigned int i;
    midi_event_t *event;
    boolean all_finished;

    if (!pcs_music_initialized || !music_playing || music_paused)
    {
        return;
    }

    all_finished = true;

    for (i = 0; i < music_num_tracks; ++i)
    {
        uint64_t elapsed_us;

        if (music_tracks[i].finished)
        {
            continue;
        }

        all_finished = false;
        elapsed_us = PCSOUND_MUSIC_TICK_US;

        // Process all events due during this callback. This also handles
        // the zero-delta events commonly found between MIDI notes.
        while (!music_tracks[i].finished && elapsed_us >= music_tracks[i].delay_us)
        {
            elapsed_us -= music_tracks[i].delay_us;

            if (!MIDI_GetNextEvent(music_tracks[i].iter, &event))
            {
                music_tracks[i].finished = true;
                break;
            }

            PCSMusicProcessEvent(event);

            if (event->event_type == MIDI_EVENT_META
             && event->data.meta.type == MIDI_META_END_OF_TRACK)
            {
                music_tracks[i].finished = true;
                break;
            }

            music_tracks[i].delay_us = PCSMusicTicksToMicroseconds(
                MIDI_GetDeltaTime(music_tracks[i].iter));
        }

        if (!music_tracks[i].finished && elapsed_us > 0)
        {
            if (music_tracks[i].delay_us > elapsed_us)
            {
                music_tracks[i].delay_us -= elapsed_us;
            }
            else
            {
                music_tracks[i].delay_us = 0;
            }
        }
    }

    for (i = 0; i < music_num_tracks; ++i)
    {
        if (!music_tracks[i].finished)
        {
            all_finished = false;
            break;
        }
    }

    if (all_finished)
    {
        if (music_looping)
        {
            music_tempo = PCSOUND_MUSIC_DEFAULT_TEMPO;
            PCSMusicStartTracks();
        }
        else
        {
            music_playing = false;
            PCSMusicClearNotes();
        }
    }
    else
    {
        music_frequency = PCSMusicCurrentFrequency();
    }
}

static void PCSCallbackFunc(int *duration, int *freq)
{
    unsigned int tone;

    *duration = PCSOUND_MUSIC_TICK_US / 1000;

    if (SDL_LockMutex(sound_lock) < 0)
    {
        *freq = 0;
        return;
    }

    PCSMusicAdvance();

    if (current_sound_lump != NULL && current_sound_remaining > 0)
    {
        // Read the next tone

        tone = *current_sound_pos;

        // Use the tone -> frequency lookup table.  See pcspkr10.zip
        // for a full discussion of this.
        // Check we don't overflow the frequency table.

        if (tone < arrlen(divisors) && divisors[tone] != 0)
        {
            *freq = (int) (TIMER_FREQ / divisors[tone]);
        }
        else
        {
            *freq = 0;
        }

        ++current_sound_pos;
        --current_sound_remaining;
    }
    else if (music_playing && !music_paused && music_volume > 0)
    {
        *freq = music_frequency;
    }
    else
    {
        *freq = 0;
    }

    SDL_UnlockMutex(sound_lock);
}

static boolean CachePCSLump(sfxinfo_t *sfxinfo)
{
    int lumplen;
    int headerlen;

    // Free the current sound lump back to the cache
 
    if (current_sound_lump != NULL)
    {
        W_ReleaseLumpNum(current_sound_lump_num);
        current_sound_lump = NULL;
    }

    // Load from WAD

    current_sound_lump = W_CacheLumpNum(sfxinfo->lumpnum, PU_STATIC);
    lumplen = W_LumpLength(sfxinfo->lumpnum);

    // Read header
  
    if (current_sound_lump[0] != 0x00 || current_sound_lump[1] != 0x00)
    {
        return false;
    }

    headerlen = (current_sound_lump[3] << 8) | current_sound_lump[2];

    if (headerlen > lumplen - 4)
    {
        return false;
    }

    // Header checks out ok

    current_sound_remaining = headerlen;
    current_sound_pos = current_sound_lump + 4;
    current_sound_lump_num = sfxinfo->lumpnum;

    return true;
}

// These Doom PC speaker sounds are not played - this can be seen in the 
// Heretic source code, where there are remnants of this left over
// from Doom.

static boolean IsDisabledSound(sfxinfo_t *sfxinfo)
{
    int i;
    const char *disabled_sounds[] = {
        "posact",
        "bgact",
        "dmact",
        "dmpain",
        "popain",
        "sawidl",
        "rifle",
    };

    // [crispy] Restore missing assault rifle PC speaker sound.
    if (gamemission == strife && crispy->soundfix)
    {
        return false;
    }

    for (i=0; i<arrlen(disabled_sounds); ++i)
    {
        if (!strcmp(sfxinfo->name, disabled_sounds[i]))
        {
            return true;
        }
    }

    return false;
}

static int I_PCS_StartSound(sfxinfo_t *sfxinfo,
                            int channel,
                            int vol,
                            int sep,
                            int pitch)
{
    int result;

    if (!pcs_sound_initialized)
    {
        return -1;
    }

    if (IsDisabledSound(sfxinfo))
    {
        return -1;
    }

    if (SDL_LockMutex(sound_lock) < 0)
    {
        return -1;
    }

    result = CachePCSLump(sfxinfo);

    if (result)
    {
        current_sound_handle = channel;
    }

    SDL_UnlockMutex(sound_lock);

    if (result)
    {
        return channel;
    }
    else
    {
        return -1;
    }
}

static void I_PCS_StopSound(int handle)
{
    if (!pcs_sound_initialized)
    {
        return;
    }

    if (SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    // If this is the channel currently playing, immediately end it.

    if (current_sound_handle == handle)
    {
        current_sound_remaining = 0;
    }
    
    SDL_UnlockMutex(sound_lock);
}

//
// Retrieve the raw data lump index
//  for a given SFX name.
//

static int I_PCS_GetSfxLumpNum(sfxinfo_t* sfx)
{
    char namebuf[9];

    if (gamemission == doom || gamemission == strife)
    {
        M_snprintf(namebuf, sizeof(namebuf), "dp%s", DEH_String(sfx->name));

        if (gamemission == strife && W_CheckNumForName(namebuf) == -1)
        {
            // Missing sounds replaced with DPRIFLE.
            M_snprintf(namebuf, sizeof(namebuf), "dp%s", DEH_String("rifle"));
        }
    }
    else
    {
        M_StringCopy(namebuf, DEH_String(sfx->name), sizeof(namebuf));
    }

     // [crispy] make missing sounds non-fatal
    return W_CheckNumForName(namebuf);
}


static boolean I_PCS_SoundIsPlaying(int handle)
{
    if (!pcs_sound_initialized)
    {
        return false;
    }

    if (handle != current_sound_handle)
    {
        return false;
    }

    return current_sound_lump != NULL && current_sound_remaining > 0;
}

static boolean PCSound_InitBackend(void)
{
    if (pcs_backend_initialized)
    {
        return true;
    }

    sound_lock = SDL_CreateMutex();

    if (sound_lock == NULL)
    {
        return false;
    }

    PCSound_SetSampleRate(snd_samplerate);
    pcs_backend_initialized = PCSound_Init(PCSCallbackFunc);

    if (!pcs_backend_initialized)
    {
        SDL_DestroyMutex(sound_lock);
        sound_lock = NULL;
    }

    return pcs_backend_initialized;
}

static void PCSound_ShutdownBackend(void)
{
    if (!pcs_backend_initialized)
    {
        return;
    }

    PCSound_Shutdown();
    pcs_backend_initialized = false;
    SDL_DestroyMutex(sound_lock);
    sound_lock = NULL;
}

static boolean I_PCS_InitSound(GameMission_t mission)
{
    gamemission = mission;

    pcs_sound_initialized = PCSound_InitBackend();
    return pcs_sound_initialized;
}

static void I_PCS_ShutdownSound(void)
{
    if (pcs_sound_initialized)
    {
        pcs_sound_initialized = false;

        if (!pcs_music_initialized)
        {
            PCSound_ShutdownBackend();
        }
    }
}

static boolean ConvertPCSMus(byte *musdata, int len, char *filename)
{
    MEMFILE *instream;
    MEMFILE *outstream;
    void *outbuf;
    size_t outbuf_len;
    int result;

    instream = mem_fopen_read(musdata, len);
    outstream = mem_fopen_write();

    result = mus2mid(instream, outstream);

    if (result == 0)
    {
        mem_get_buf(outstream, &outbuf, &outbuf_len);
        result = M_WriteFile(filename, outbuf, outbuf_len) ? 0 : -1;
    }

    mem_fclose(instream);
    mem_fclose(outstream);

    return result == 0;
}

static void *I_PCS_RegisterSong(void *data, int len)
{
    midi_file_t *result;
    char *filename;

    if (!pcs_music_initialized)
    {
        return NULL;
    }

    filename = M_TempFile("doom.mid");

    if (IsMid(data, len))
    {
        if (!M_WriteFile(filename, data, len))
        {
            M_remove(filename);
            free(filename);
            return NULL;
        }
    }
    else if (!IsMus(data, len) || !ConvertPCSMus(data, len, filename))
    {
        M_remove(filename);
        free(filename);
        return NULL;
    }

    result = MIDI_LoadFile(filename);
    M_remove(filename);
    free(filename);

    return result;
}

static void I_PCS_UnRegisterSong(void *handle)
{
    if (handle == NULL)
    {
        return;
    }

    if (SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    if (handle == current_music_file)
    {
        PCSMusicStop();
        current_music_file = NULL;
    }

    SDL_UnlockMutex(sound_lock);
    MIDI_FreeFile(handle);
}

static void I_PCS_PlaySong(void *handle, boolean looping)
{
    unsigned int i;

    if (!pcs_music_initialized || handle == NULL
     || SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    PCSMusicStop();
    current_music_file = handle;
    music_num_tracks = MIDI_NumTracks(current_music_file);
    music_ticks_per_beat = MIDI_GetFileTimeDivision(current_music_file);
    music_tempo = PCSOUND_MUSIC_DEFAULT_TEMPO;
    music_looping = looping;

    if (music_num_tracks == 0 || music_ticks_per_beat == 0)
    {
        current_music_file = NULL;
        SDL_UnlockMutex(sound_lock);
        return;
    }

    music_tracks = calloc(music_num_tracks, sizeof(*music_tracks));

    if (music_tracks == NULL)
    {
        music_num_tracks = 0;
        current_music_file = NULL;
        SDL_UnlockMutex(sound_lock);
        return;
    }

    for (i = 0; i < music_num_tracks; ++i)
    {
        music_tracks[i].iter = MIDI_IterateTrack(current_music_file, i);

        if (music_tracks[i].iter == NULL)
        {
            PCSMusicStop();
            current_music_file = NULL;
            SDL_UnlockMutex(sound_lock);
            return;
        }
    }

    PCSMusicStartTracks();
    music_playing = true;
    music_paused = false;

    SDL_UnlockMutex(sound_lock);
}

static void I_PCS_StopSong(void)
{
    if (!pcs_music_initialized || SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    PCSMusicStop();
    SDL_UnlockMutex(sound_lock);
}

static void I_PCS_SetMusicVolume(int volume)
{
    if (!pcs_music_initialized || SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    music_volume = volume;
    SDL_UnlockMutex(sound_lock);
}

static void I_PCS_PauseSong(void)
{
    if (!pcs_music_initialized || SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    music_paused = true;
    SDL_UnlockMutex(sound_lock);
}

static void I_PCS_ResumeSong(void)
{
    if (!pcs_music_initialized || SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    music_paused = false;
    SDL_UnlockMutex(sound_lock);
}

static boolean I_PCS_MusicIsPlaying(void)
{
    boolean result;

    if (!pcs_music_initialized || SDL_LockMutex(sound_lock) < 0)
    {
        return false;
    }

    result = music_playing;
    SDL_UnlockMutex(sound_lock);

    return result;
}

static void I_PCS_ShutdownMusic(void)
{
    if (!pcs_music_initialized)
    {
        return;
    }

    if (SDL_LockMutex(sound_lock) < 0)
    {
        return;
    }

    PCSMusicStop();
    current_music_file = NULL;
    SDL_UnlockMutex(sound_lock);

    pcs_music_initialized = false;

    if (!pcs_sound_initialized)
    {
        PCSound_ShutdownBackend();
    }
}

static boolean I_PCS_InitMusic(void)
{
    pcs_music_initialized = PCSound_InitBackend();
    return pcs_music_initialized;
}

static const snddevice_t music_pcsound_devices[] =
{
    SNDDEVICE_PCSPEAKER,
};

const music_module_t music_pcsound_module =
{
    music_pcsound_devices,
    arrlen(music_pcsound_devices),
    I_PCS_InitMusic,
    I_PCS_ShutdownMusic,
    I_PCS_SetMusicVolume,
    I_PCS_PauseSong,
    I_PCS_ResumeSong,
    I_PCS_RegisterSong,
    I_PCS_UnRegisterSong,
    I_PCS_PlaySong,
    I_PCS_StopSong,
    I_PCS_MusicIsPlaying,
    NULL,
};

static void I_PCS_UpdateSound(void)
{
    // no-op.
}

void I_PCS_UpdateSoundParams(int channel, int vol, int sep)
{
    // no-op.
}

static const snddevice_t sound_pcsound_devices[] =
{
    SNDDEVICE_PCSPEAKER,
};

const sound_module_t sound_pcsound_module =
{
    sound_pcsound_devices,
    arrlen(sound_pcsound_devices),
    I_PCS_InitSound,
    I_PCS_ShutdownSound,
    I_PCS_GetSfxLumpNum,
    I_PCS_UpdateSound,
    I_PCS_UpdateSoundParams,
    I_PCS_StartSound,
    I_PCS_StopSound,
    I_PCS_SoundIsPlaying,
};
