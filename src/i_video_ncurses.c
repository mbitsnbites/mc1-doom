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

#include <stdlib.h>
#include <ncurses.h>

static const char COL_TO_CHAR[] = " .,-=:;oO0";
#define COL_RANGE (sizeof (COL_TO_CHAR) - 1)

static boolean do_color;
static int num_colors;
static short palette_lut[256];
static byte short_palette[256 * 3];  // Actually num_colors * 3 entries.

static int sqr_diff (int a, int b)
{
    int diff = a - b;
    return diff * diff;
}

void I_InitGraphics (void)
{
    screens[0] = (unsigned char*)malloc (SCREENWIDTH * SCREENHEIGHT);
    initscr ();

    do_color = false;
    if (has_colors () == TRUE)
    {
        start_color ();
        num_colors = (COLORS < 256) ? COLORS : 256;
        num_colors =
            (COLOR_PAIRS - 1 < num_colors) ? COLOR_PAIRS - 1 : num_colors;
        do_color = true;
    }

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
    int col_div;

    if (!do_color)
        return;

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

        // Generate a curses color & pair.
        r = (1000 * r) / 255;
        g = (1000 * g) / 255;
        b = (1000 * b) / 255;
        init_color (i, r, g, b);
        init_pair (i + 1, COLOR_BLACK, i);
    }

    // Define the optimal palette -> short_palette LUT.
    for (i = 0; i < 256; ++i)
    {
        // The color that we want.
        r = (int)palette[i * 3 + 0];
        g = (int)palette[i * 3 + 1];
        b = (int)palette[i * 3 + 2];

        // Find the closest match.
        best_err = 99999999;
        best_idx = 0;
        for (j = 0; j < num_colors; ++j)
        {
            err = sqr_diff (r, (int)short_palette[j * 3 + 0]) +
                  sqr_diff (g, (int)short_palette[j * 3 + 1]) +
                  sqr_diff (b, (int)short_palette[j * 3 + 2]);
            if (err < best_err)
            {
                best_idx = j;
                best_err = err;
            }
        }
        palette_lut[i] = best_idx;
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
    char c[2];
    c[1] = 0;

    // Get consol size.
    text_width = COLS;
    text_height = LINES;

    // We just do nearest neigbour sampling.
    for (y = 0; y < text_height; ++y)
    {
        v = (y * SCREENHEIGHT) / text_height;
        for (x = 0; x < text_width; ++x)
        {
            u = (x * SCREENWIDTH) / text_width;
            color = (int)screens[0][v * SCREENWIDTH + u];
            if (do_color)
            {
                // Pick a color from the palette and print it as a space char.
                color = palette_lut[color];
                attron (COLOR_PAIR (1 + color));
                c[0] = ' ';
                mvwprintw (stdscr, y, x, c);
                attroff (COLOR_PAIR (1 + color));
            }
            else
            {
                // Convert the color to a character, and print it.
                c[0] = (char)COL_TO_CHAR[(color * COL_RANGE) / 255];
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
