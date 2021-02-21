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
#include <mr32intrin.h>

#include "doomdef.h"
#include "d_event.h"
#include "d_main.h"
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

// MC1 MMIO.
// clang-format off
#define CLKCNTLO    0
#define CLKCNTHI    4
#define CPUCLK      8
#define VRAMSIZE    12
#define XRAMSIZE    16
#define VIDWIDTH    20
#define VIDHEIGHT   24
#define VIDFPS      28
#define VIDFRAMENO  32
#define VIDY        36
#define SWITCHES    40
#define BUTTONS     44
#define KEYPTR      48
#define MOUSEPOS    52
#define MOUSEBTNS   56
// clang-format on

#define GET_MMIO(reg) (*(volatile unsigned*)(&((volatile byte*)0xc0000000)[reg]))
#define GET_KEYBUF(ptr) ((volatile unsigned*)(((volatile byte*)0xc0000080)))[ptr]
#define KEYBUF_SIZE 16

// MC1 keyboard scancodes.
// clang-format off
#define KB_A                0x01c
#define KB_B                0x032
#define KB_C                0x021
#define KB_D                0x023
#define KB_E                0x024
#define KB_F                0x02b
#define KB_G                0x034
#define KB_H                0x033
#define KB_I                0x043
#define KB_J                0x03b
#define KB_K                0x042
#define KB_L                0x04b
#define KB_M                0x03a
#define KB_N                0x031
#define KB_O                0x044
#define KB_P                0x04d
#define KB_Q                0x015
#define KB_R                0x02d
#define KB_S                0x01b
#define KB_T                0x02c
#define KB_U                0x03c
#define KB_V                0x02a
#define KB_W                0x01d
#define KB_X                0x022
#define KB_Y                0x035
#define KB_Z                0x01a
#define KB_0                0x045
#define KB_1                0x016
#define KB_2                0x01e
#define KB_3                0x026
#define KB_4                0x025
#define KB_5                0x02e
#define KB_6                0x036
#define KB_7                0x03d
#define KB_8                0x03e
#define KB_9                0x046

#define KB_SPACE            0x029
#define KB_BACKSPACE        0x066
#define KB_TAB              0x00d
#define KB_LSHIFT           0x012
#define KB_LCTRL            0x014
#define KB_LALT             0x011
#define KB_LMETA            0x11f
#define KB_RSHIFT           0x059
#define KB_RCTRL            0x114
#define KB_RALT             0x111
#define KB_RMETA            0x127
#define KB_ENTER            0x05a
#define KB_ESC              0x076
#define KB_F1               0x005
#define KB_F2               0x006
#define KB_F3               0x004
#define KB_F4               0x00c
#define KB_F5               0x003
#define KB_F6               0x00b
#define KB_F7               0x083
#define KB_F8               0x00a
#define KB_F9               0x001
#define KB_F10              0x009
#define KB_F11              0x078
#define KB_F12              0x007

#define KB_INSERT           0x170
#define KB_HOME             0x16c
#define KB_DEL              0x171
#define KB_END              0x169
#define KB_PGUP             0x17d
#define KB_PGDN             0x17a
#define KB_UP               0x175
#define KB_LEFT             0x16b
#define KB_DOWN             0x172
#define KB_RIGHT            0x174

#define KB_KP_0             0x070
#define KB_KP_1             0x069
#define KB_KP_2             0x072
#define KB_KP_3             0x07a
#define KB_KP_4             0x06b
#define KB_KP_5             0x073
#define KB_KP_6             0x074
#define KB_KP_7             0x06c
#define KB_KP_8             0x075
#define KB_KP_9             0x07d
#define KB_KP_PERIOD        0x071
#define KB_KP_PLUS          0x079
#define KB_KP_MINUS         0x07b
#define KB_KP_MUL           0x07c
#define KB_KP_DIV           0x06d
#define KB_KP_ENTER         0x06e

#define KB_ACPI_POWER       0x137
#define KB_ACPI_SLEEP       0x13f
#define KB_ACPI_WAKE        0x15e

#define KB_MM_NEXT_TRACK    0x14d
#define KB_MM_PREV_TRACK    0x115
#define KB_MM_STOP          0x13b
#define KB_MM_PLAY_PAUSE    0x134
#define KB_MM_MUTE          0x123
#define KB_MM_VOL_UP        0x132
#define KB_MM_VOL_DOWN      0x121
#define KB_MM_MEDIA_SEL     0x150
#define KB_MM_EMAIL         0x148
#define KB_MM_CALCULATOR    0x12b
#define KB_MM_MY_COMPUTER   0x140

#define KB_WWW_SEARCH       0x110
#define KB_WWW_HOME         0x13a
#define KB_WWW_BACK         0x138
#define KB_WWW_FOWRARD      0x130
#define KB_WWW_STOP         0x128
#define KB_WWW_REFRESH      0x120
#define KB_WWW_FAVORITES    0x118
// clang-format on

static void I_MC1_CreateVCP (byte* vcp)
{
    // TODO(m): Implement me!
    (void)vcp;
}

static int I_MC1_TranslateKey (unsigned keycode)
{
    // clang-format off
    switch (keycode)
    {
        case KB_SPACE:         return ' ';
        case KB_LEFT:          return KEY_LEFTARROW;
        case KB_RIGHT:         return KEY_RIGHTARROW;
        case KB_DOWN:          return KEY_DOWNARROW;
        case KB_UP:            return KEY_UPARROW;
        case KB_ESC:           return KEY_ESCAPE;
        case KB_ENTER:         return KEY_ENTER;
        case KB_TAB:           return KEY_TAB;
        case KB_F1:            return KEY_F1;
        case KB_F2:            return KEY_F2;
        case KB_F3:            return KEY_F3;
        case KB_F4:            return KEY_F4;
        case KB_F5:            return KEY_F5;
        case KB_F6:            return KEY_F6;
        case KB_F7:            return KEY_F7;
        case KB_F8:            return KEY_F8;
        case KB_F9:            return KEY_F9;
        case KB_F10:           return KEY_F10;
        case KB_F11:           return KEY_F11;
        case KB_F12:           return KEY_F12;
        case KB_DEL:
        case KB_BACKSPACE:     return KEY_BACKSPACE;
        case KB_MM_PLAY_PAUSE: return KEY_PAUSE;
        case KB_KP_PLUS:       return KEY_EQUALS;
        case KB_KP_MINUS:      return KEY_MINUS;
        case KB_LSHIFT:
        case KB_RSHIFT:        return KEY_RSHIFT;
        case KB_LCTRL:
        case KB_RCTRL:         return KEY_RCTRL;
        case KB_LALT:
        case KB_LMETA:
        case KB_RALT:
        case KB_RMETA:         return KEY_RALT;

        case KB_A:             return 'a';
        case KB_B:             return 'b';
        case KB_C:             return 'c';
        case KB_D:             return 'd';
        case KB_E:             return 'e';
        case KB_F:             return 'f';
        case KB_G:             return 'g';
        case KB_H:             return 'h';
        case KB_I:             return 'i';
        case KB_J:             return 'j';
        case KB_K:             return 'k';
        case KB_L:             return 'l';
        case KB_M:             return 'm';
        case KB_N:             return 'n';
        case KB_O:             return 'o';
        case KB_P:             return 'p';
        case KB_Q:             return 'q';
        case KB_R:             return 'r';
        case KB_S:             return 's';
        case KB_T:             return 't';
        case KB_U:             return 'u';
        case KB_V:             return 'v';
        case KB_W:             return 'w';
        case KB_X:             return 'x';
        case KB_Y:             return 'y';
        case KB_Z:             return 'z';
        case KB_0:             return '0';
        case KB_1:             return '1';
        case KB_2:             return '2';
        case KB_3:             return '3';
        case KB_4:             return '4';
        case KB_5:             return '5';
        case KB_6:             return '6';
        case KB_7:             return '7';
        case KB_8:             return '8';
        case KB_9:             return '9';

        default:
            return 0;
    }
    // clang-format on
}

static unsigned s_keyptr;

static boolean I_MC1_PollKeyEvent (event_t* event)
{
    unsigned keyptr, keycode;
    int doom_key;

    // Check if we have any new keycode from the keyboard.
    keyptr = GET_MMIO(KEYPTR);
    if (s_keyptr == keyptr)
        return false;

    // Get the next keycode.
    ++s_keyptr;
    keycode = GET_KEYBUF(s_keyptr % KEYBUF_SIZE);

    // Translate the MC1 keycode to a Doom keycode.
    doom_key = I_MC1_TranslateKey (keycode & 0x1ff);
    if (doom_key != 0) {
        // Create a Doom keyboard event.
        event->type = (keycode & 0x80000000) ? ev_keydown : ev_keyup;
        event->data1 = doom_key;
    }

    return true;
}

static boolean I_MC1_PollMouseEvent (event_t* event)
{
    static unsigned s_old_mousepos;
    static unsigned s_old_mousebtns;

    // Do we have a new mouse movement event?
    unsigned mousepos = GET_MMIO(MOUSEPOS);
    unsigned mousebtns = GET_MMIO(MOUSEBTNS);
    if (mousepos == s_old_mousepos && mousebtns == s_old_mousebtns)
        return false;

    // Get the x & y mouse delta.
    int mousex = ((int)(short)mousepos) - ((int)(short)s_old_mousepos);
    int mousey = (((int)mousepos) >> 16) - (((int)s_old_mousepos) >> 16);

    // Create a Doom mouse event.
    event->type = ev_mouse;
    event->data1 = mousebtns;
    event->data2 = mousex;
    event->data3 = mousey;

    s_old_mousepos = mousepos;
    s_old_mousebtns = mousebtns;

    return true;
}

void I_InitGraphics (void)
{
    // Allocate regular memory for the Doom screen.
    // TODO(m): We should use VRAM if there's enough room, as it's faster.
    screens[0] = (unsigned char*)malloc (SCREENWIDTH * SCREENHEIGHT);
    if (screens[0] == NULL)
        I_Error ("Couldn't allocate screen memory");

    I_MC1_CreateVCP (VRAM_VCP);

    s_keyptr = GET_MMIO(KEYPTR);

    printf (
        "I_InitGraphics: Framebuffer @ 0x%08x (%d)\n"
        "                Palette     @ 0x%08x (%d)\n",
        (unsigned)VRAM_FB,
        (unsigned)VRAM_FB,
        (unsigned)VRAM_PAL,
        (unsigned)VRAM_PAL);
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
    event_t event;

    // Read mouse and keyboard events.
    if (I_MC1_PollKeyEvent (&event))
        D_PostEvent (&event);
    if (I_MC1_PollMouseEvent (&event))
        D_PostEvent (&event);
}

void I_SetPalette (byte* palette)
{
    unsigned* dst = (unsigned*)VRAM_PAL;
    const unsigned a = 255;
    for (int i = 0; i < 256; ++i)
    {
        unsigned r = (unsigned)palette[i * 3];
        unsigned g = (unsigned)palette[i * 3 + 1];
        unsigned b = (unsigned)palette[i * 3 + 2];
#ifdef __MRISC32_PACKED_OPS__
        dst[i] = _mr32_pack_h (_mr32_pack (a, g), _mr32_pack (b, r));
#else
        dst[i] = (a << 24) | (b << 16) | (g << 8) | r;
#endif
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
