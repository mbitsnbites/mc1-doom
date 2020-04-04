// Emacs style mode select   -*- C++ -*- 
//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 1993-1996 by id Software, Inc.
// Copyright (C) 2020 by Marcus Geelnard
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
//	System interface for sound.
//
//-----------------------------------------------------------------------------

#include "i_sound.h"

//
// SFX API.
// This is currently an empty implementation. No sound support yet.
//

void I_SetChannels()
{
}	
 
void I_SetSfxVolume(int volume)
{
    (void)volume;
}

void I_SetMusicVolume(int volume)
{
    (void)volume;
}

int I_GetSfxLumpNum(sfxinfo_t* sfx)
{
    // Do we need to throw I_Error here?
    (void)sfx;
    return -1;
}

int I_StartSound(int id,
                 int vol,
                 int sep,
                 int pitch,
                 int priority)
{
    (void)vol;
    (void)sep;
    (void)pitch;
    (void)priority;
    return id;
}

void I_StopSound (int handle)
{
    (void)handle;
}


int I_SoundIsPlaying(int handle)
{
    return 0;
}

void I_UpdateSound(void)
{
}

void I_SubmitSound(void)
{
}

void I_UpdateSoundParams(int handle,
                         int vol,
                         int sep,
                         int pitch)
{
    (void)handle;
    (void)vol;
    (void)sep;
    (void)pitch;
}

void I_ShutdownSound(void)
{    
}

void I_InitSound(void)
{ 
}


//
// MUSIC API.
// This is currently an empty implementation. No sound support yet.
//

void I_InitMusic(void)
{
}

void I_ShutdownMusic(void)
{
}

void I_PlaySong(int handle, int looping)
{
    (void)handle;
    (void)looping;
}

void I_PauseSong (int handle)
{
    (void)handle;
}

void I_ResumeSong (int handle)
{
    (void)handle;
}

void I_StopSong(int handle)
{
    (void)handle;
}

void I_UnRegisterSong(int handle)
{
    (void)handle;
}

int I_RegisterSong(void* data)
{
    (void)data;
    return 1;
}

int I_QrySongPlaying(int handle)
{
    (void)handle;
    return 0;
}

