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

#include "i_video.h"
#include "v_video.h"
#include "doomdef.h"

#include <math.h>
#include <ncurses.h>
#include <stdlib.h>

static const char COL_TO_CHAR[] = " .-=+oq*O0X8@#";

static boolean do_color;
static boolean use_custom_colors;
static int num_colors;
static short palette_lut[256];
static byte short_palette[256 * 3];  // Actually num_colors * 3 entries.

static byte gamma_lut[256];
#define INV_GAMMA (1.0f / 1.5f)

static int sqr_diff (int a, int b)
{
    int diff = a - b;
    return diff * diff;
}

static void rgb_to_yuv (int r, int g, int b, int* y, int* u, int* v)
{
    *y = ((66 * r + 129 * g + 25 * b + 128) >> 8) + 16;
    *u = ((-38 * r - 74 * g + 112 * b + 128) >> 8) + 128;
    *v = ((112 * r - 94 * g - 18 * b + 128) >> 8) + 128;
}

static int color_diff (int r1, int g1, int b1, int r2, int g2, int b2)
{
#if 1
    // TODO(m): Is YUV the best color space for this?
    int y1, u1, v1;
    int y2, u2, v2;
    rgb_to_yuv (r1, g1, b1, &y1, &u1, &v1);
    rgb_to_yuv (r2, g2, b2, &y2, &u2, &v2);

    // We weigh the Y component stronger.
    return 3 * sqr_diff (y1, y2) + sqr_diff (u1, u2) + sqr_diff (v1, v2);
#else
    return sqr_diff (r1, r2) + sqr_diff (g1, g2) + sqr_diff (b1, b2);
#endif
}

void I_InitGraphics (void)
{
    int i;

    // Allocate memory for the framebuffer.
    screens[0] = (unsigned char*)malloc (SCREENWIDTH * SCREENHEIGHT);

    initscr ();
    cbreak ();
    noecho ();
    intrflush (stdscr, FALSE);
    keypad (stdscr, TRUE);

    // Detect color capabilities.
    do_color = false;
    use_custom_colors = false;
    if (has_colors () == TRUE)
    {
        start_color ();
        num_colors = ((COLOR_PAIRS - 1) < 256) ? (COLOR_PAIRS - 1) : 256;
        num_colors = (COLORS < num_colors) ? COLORS : num_colors;

#if !defined(__APPLE__)
        // init_color() appears to have no effect on macOS terminals.
        if (can_change_color () == TRUE)
            use_custom_colors = true;
#endif

        for (i = 0; i < num_colors; ++i)
        {
            if (!use_custom_colors)
            {
                // Query the curses color.
                short rs;
                short gs;
                short bs;
                color_content ((short)i, &rs, &gs, &bs);
                short_palette[i * 3 + 0] = (byte) ((255 * (int)rs) / 1000);
                short_palette[i * 3 + 1] = (byte) ((255 * (int)gs) / 1000);
                short_palette[i * 3 + 2] = (byte) ((255 * (int)bs) / 1000);
            }

            // Generate a curses pair.
            init_pair (i + 1, i, i);
        }

        do_color = true;
    }
    else
    {
        num_colors = (int)(sizeof (COL_TO_CHAR) - 1);
        for (i = 0; i < num_colors; ++i)
        {
            byte col = (byte) ((255 * i) / (num_colors - 1));
            short_palette[i * 3 + 0] = col;
            short_palette[i * 3 + 1] = col;
            short_palette[i * 3 + 2] = col;
        }
    }

    // Initialize the gamma LUT (we want to brighten the colors).
    for (i = 0; i < 256; ++i)
        gamma_lut[i] = (byte) (powf ((float)i / 255.0f, INV_GAMMA) * 255.0f);

// Redirect stderr to /dev/null.
// TODO(m): Maybe redirect to a pipe and show it in I_ShutdownGraphics?
#ifdef _WIN32
    freopen ("nul:", "a", stderr);  // Unlikely to ever happen...
#else
    freopen ("/dev/null", "a", stderr);
#endif
}

void I_ShutdownGraphics (void)
{
    endwin ();
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
    int pal_idx;
    int next_pal_idx;
    int i;
    int j;
    int best_idx;
    int best_err;
    int err;
    int r;
    int g;
    int b;
    short rs;
    short gs;
    short bs;
    int col_div;

    if (use_custom_colors)
    {
        // Create a smaller palette consisting of num_colors.
        next_pal_idx = 0;
        for (i = 0; i < num_colors; ++i)
        {
            pal_idx = next_pal_idx;
            next_pal_idx = ((i + 1) * 256) / num_colors;

            // Calculate the average color for a number of palette entries.
            // Note: This assumes that the palette is reasonably smooth.
            r = 0;
            g = 0;
            b = 0;
            for (j = pal_idx; j < next_pal_idx; ++j)
            {
                r += (int)*palette++;
                g += (int)*palette++;
                b += (int)*palette++;
            }
            col_div = next_pal_idx - pal_idx;
            r = (r + (col_div >> 1)) / col_div;
            g = (g + (col_div >> 1)) / col_div;
            b = (b + (col_div >> 1)) / col_div;

            // Store the average color in the short palette.
            short_palette[i * 3 + 0] = (byte)r;
            short_palette[i * 3 + 1] = (byte)g;
            short_palette[i * 3 + 2] = (byte)b;

            // Generate a curses color.
            rs = (short)((1000 * r) / 255);
            gs = (short)((1000 * g) / 255);
            bs = (short)((1000 * b) / 255);
            init_color ((short)i, rs, gs, bs);
        }
    }

    // Define the optimal palette -> short_palette LUT.
    for (i = 0; i < 256; ++i)
    {
        // The color that we want.
        r = (int)gamma_lut[palette[i * 3 + 0]];
        g = (int)gamma_lut[palette[i * 3 + 1]];
        b = (int)gamma_lut[palette[i * 3 + 2]];

        // Find the closest match.
        best_err = 99999999;
        best_idx = 0;
        for (j = 0; j < num_colors; ++j)
        {
            err = color_diff (r,
                              g,
                              b,
                              (int)short_palette[j * 3 + 0],
                              (int)short_palette[j * 3 + 1],
                              (int)short_palette[j * 3 + 2]);
            if (err < best_err)
            {
                best_idx = j;
                best_err = err;
            }
        }
        palette_lut[i] = (short)best_idx;
    }
}

void I_UpdateNoBlit (void)
{
}

void I_FinishUpdate (void)
{
    int text_width;
    int text_height;
    int x;
    int y;
    int u;
    int v;
    int color;
    byte* row;
    char c[2];

    c[0] = ' ';
    c[1] = 0;

    // Get consol size.
    text_width = COLS;
    text_height = LINES;

    // We just do nearest neigbour sampling.
    for (y = 0; y < text_height; ++y)
    {
        v = (y * SCREENHEIGHT) / text_height;
        row = &screens[0][v * SCREENWIDTH];
        for (x = 0; x < text_width; ++x)
        {
            u = (x * SCREENWIDTH) / text_width;
            color = (int)palette_lut[row[u]];

            if (do_color)
            {
                // Pick a color from the palette and print it as a space char.
                attron (COLOR_PAIR (1 + color));
                mvwprintw (stdscr, y, x, c);
                attroff (COLOR_PAIR (1 + color));
            }
            else
            {
                // Convert the color to a character, and print it.
                c[0] = (char)COL_TO_CHAR[color];
                mvwprintw (stdscr, y, x, c);
            }
        }
    }

    refresh ();
}

void I_WaitVBL (int count)
{
    (void)count;
}

void I_ReadScreen (byte* scr)
{
    memcpy (scr, screens[0], SCREENWIDTH * SCREENHEIGHT);
}
