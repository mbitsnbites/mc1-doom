// Emacs style mode select   -*- C++ -*-
//-----------------------------------------------------------------------------
//
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
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Dummy implementations of standard C functions.
// These are placeholders for proper implementations.
//-----------------------------------------------------------------------------

void access(void)
{
}

void _close(void)
{
}

void _exit(int code)
{
    (void) code;
    while (1) {}
}

void _fstat(void)
{
}

void _getpid(void)
{
}

void _gettimeofday(void)
{
}

void _isatty(void)
{
}

void _kill(void)
{
}

void _lseek(void)
{
}

void mkdir(void)
{
}

void _open(void)
{
}

void _read(void)
{
}

void _sbrk(void)
{
}

void _write(void)
{
}

