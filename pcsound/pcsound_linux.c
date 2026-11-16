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
//    PC speaker driver for Linux.
//

#include "config.h"

#ifdef HAVE_LINUX_KD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef CRISPY_HAVE_LINUX_INPUT_H
#include <linux/input.h>
#endif

#include "SDL.h"
#include "SDL_thread.h"

#include "pcsound.h"
#include "pcsound_internal.h"

#define CONSOLE_DEVICE "/dev/console"
#ifdef CRISPY_HAVE_LINUX_INPUT_H
#define EVDEV_DEVICE "/dev/input/by-path/platform-pcspkr-event-spkr"
#endif

static int speaker_handle;
#ifdef CRISPY_HAVE_LINUX_INPUT_H
static int evdev_enabled;
#endif
static pcsound_callback_func callback;
static int sound_thread_running = 0;
static SDL_Thread *sound_thread_handle;
static int sleep_adjust = 0;

static int SetFrequency(int frequency)
{
#ifdef CRISPY_HAVE_LINUX_INPUT_H
    if (evdev_enabled)
    {
        struct input_event event;
        ssize_t written;

        memset(&event, 0, sizeof(event));
        event.type = EV_SND;
        event.code = SND_TONE;
        event.value = frequency;

        do
        {
            written = write(speaker_handle, &event, sizeof(event));
        }
        while (written < 0 && errno == EINTR);

        if (written != (ssize_t) sizeof(event))
        {
            if (written < 0)
            {
                fprintf(stderr, "PCSound_Linux: Failed to write PC speaker event: %s\n",
                        strerror(errno));
            }
            else
            {
                fprintf(stderr, "PCSound_Linux: Incomplete PC speaker event write\n");
            }

            return 0;
        }

        return 1;
    }
#endif

    {
        int cycles;

        if (frequency != 0)
        {
            cycles = PCSOUND_8253_FREQUENCY / frequency;
        }
        else
        {
            cycles = 0;
        }

        if (ioctl(speaker_handle, KIOCSOUND, cycles) < 0)
        {
            fprintf(stderr, "PCSound_Linux: Failed to set PC speaker frequency: %s\n",
                    strerror(errno));
            return 0;
        }
    }

    return 1;
}

static void AdjustedSleep(unsigned int ms)
{
    unsigned int start_time;
    unsigned int end_time;
    unsigned int actual_time;

    // Adjust based on previous error to keep the tempo right

    if (sleep_adjust > ms)
    {
        sleep_adjust -= ms;
        return;
    }
    else
    {
        ms -= sleep_adjust;
    }

    // Do the sleep and record how long it takes

    start_time = SDL_GetTicks();

    SDL_Delay(ms);
    
    end_time = SDL_GetTicks();

    if (end_time > start_time)
    {
        actual_time = end_time - start_time;
    }
    else
    {
        actual_time = ms;
    }

    if (actual_time < ms)
    {
        actual_time = ms;
    }

    // Save sleep_adjust for next time

    sleep_adjust = actual_time - ms;
}

static int SoundThread(void *unused)
{
    int current_freq = 0;
    int frequency;
    int duration;
    
    while (sound_thread_running)
    {
        callback(&duration, &frequency);

        if (current_freq != frequency)
        {
            current_freq = frequency;
            SetFrequency(frequency);
        }

        AdjustedSleep(duration);
    }

    return 0;
}

static int PCSound_Linux_Init(pcsound_callback_func callback_func)
{
#ifdef CRISPY_HAVE_LINUX_INPUT_H
    evdev_enabled = 0;
#endif

    // Prefer the evdev interface.  Unlike the legacy console interface,
    // access to this device can be granted to unprivileged users by udev.
#ifdef CRISPY_HAVE_LINUX_INPUT_H
    speaker_handle = open(EVDEV_DEVICE, O_WRONLY);

    if (speaker_handle != -1)
    {
        evdev_enabled = 1;

        if (SetFrequency(0))
        {
            goto start_thread;
        }

        close(speaker_handle);
        evdev_enabled = 0;
    }

    // Fall back to the legacy console interface.
#endif

    speaker_handle = open(CONSOLE_DEVICE, O_WRONLY);

    if (speaker_handle == -1)
    {
        // Don't have permissions for the console device?

	fprintf(stderr, "PCSound_Linux_Init: Failed to open '%s': %s\n",
			CONSOLE_DEVICE, strerror(errno));
        return 0;
    }

    if (!SetFrequency(0))
    {
        // KIOCSOUND not supported: non-PC linux?

        close(speaker_handle);
        return 0;
    }

start_thread:
    // Start a thread up to generate PC speaker output
    
    callback = callback_func;
    sound_thread_running = 1;

    sound_thread_handle =
        SDL_CreateThread(SoundThread, "PC speaker thread", NULL);

    return 1;
}

static void PCSound_Linux_Shutdown(void)
{
    sound_thread_running = 0;
    SDL_WaitThread(sound_thread_handle, NULL);
    SetFrequency(0);
    close(speaker_handle);
#ifdef CRISPY_HAVE_LINUX_INPUT_H
    evdev_enabled = 0;
#endif
}

pcsound_driver_t pcsound_linux_driver =
{
    "Linux",
    PCSound_Linux_Init,
    PCSound_Linux_Shutdown,
};

#endif /* #ifdef HAVE_LINUX_KD_H */
