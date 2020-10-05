// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 2020 Marcus Geelnard
//
// This source is available for distribution and/or modification
// only under the terms of the DOOM Source Code License as
// published by id Software. All rights reserved.
//
// The source is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// FITNESS FOR A PARTICULAR PURPOSE. See the DOOM Source Code License
// for more details.
//
// $Log:$
//
// DESCRIPTION:
//	DOOM graphics stuff for MC1.
//
//-----------------------------------------------------------------------------

#include "i_video.h"


//-----------------------------------------------------------------------------
// TODO(m): This is currently just a dummy placeholder. Implement proper video
// support!
//-----------------------------------------------------------------------------

void I_InitGraphics(void)
{
}

void I_ShutdownGraphics(void)
{
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
    (void)palette;
}

void I_UpdateNoBlit(void)
{
}

void I_FinishUpdate(void)
{
}

void I_WaitVBL(int count)
{
    (void)count;
}

void I_ReadScreen(byte* scr)
{
    (void)scr;
}

