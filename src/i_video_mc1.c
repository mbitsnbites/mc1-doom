// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 2020 by Marcus Geelnard
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
//      Dummy system interface for video.
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#include "doomdef.h"
#include "i_system.h"
#include "i_video.h"
#include "v_video.h"

// We assume that the Doom binary is loaded into XRAM (0x80000000...), or
// into the "ROM" (0x00000000...) for the simulator, and that it has complete
// ownership of VRAM (0x40000000...). Hence we just hard-code the video
// addresses.
#define VRAM_BASE 0x40000100
#define VCP_SIZE (4*(16+SCREENHEIGHT*2))
#define PAL_SIZE (4*(256+1))
static byte* const VRAM_VCP = (byte*)VRAM_BASE;
static byte* const VRAM_PAL = (byte*)(VRAM_BASE + VCP_SIZE);
static byte* const VRAM_FB = (byte*)(VRAM_BASE + VCP_SIZE + PAL_SIZE);

void I_InitGraphics (void)
{
    // Allocate regular memory for the Doom screen.
    screens[0] = (unsigned char*)malloc (SCREENWIDTH * SCREENHEIGHT);
    if (screens[0] == NULL)
        I_Error ("Couldn't allocate screen memory");

    // TODO(m): Create the VCP.
    (void)VRAM_VCP;

    printf ("I_InitGraphics: VRAM_FB=%d, VRAM_PAL=%d\n",
            (int)VRAM_FB,
            (int)VRAM_PAL);
}

void I_ShutdownGraphics (void)
{
    free (screens[0]);
}

void I_StartFrame (void)
{
    // Er? This is declared in i_system.h.
}

void I_StartTic (void)
{
    // Er? This is declared in i_system.h.
}

void I_SetPalette (byte* palette)
{
    for (int i = 0; i < 256; ++i)
    {
        VRAM_PAL[i * 4] = palette[i * 3];
        VRAM_PAL[i * 4 + 1] = palette[i * 3 + 1];
        VRAM_PAL[i * 4 + 2] = palette[i * 3 + 2];
        VRAM_PAL[i * 4 + 3] = 255;
    }
}

void I_UpdateNoBlit (void)
{
}

void I_FinishUpdate (void)
{
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#pragma GCC diagnostic ignored "-Wstringop-overflow"
#endif
    memcpy (VRAM_FB, screens[0], SCREENWIDTH * SCREENHEIGHT);
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
}

void I_WaitVBL (int count)
{
    // TODO(m): Replace this with MC1 MMIO-based timing.
    struct timeval t0, t;
    long dt, waitt;

    // Busy-wait for COUNT*1/60 s.
    gettimeofday (&t0, NULL);
    waitt = count * (1000000L / 60L);
    do
    {
        gettimeofday (&t, NULL);
        dt = (t.tv_sec - t0.tv_sec) * 1000000L + (t.tv_usec - t0.tv_usec);
    } while (dt < waitt);
}

void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}
