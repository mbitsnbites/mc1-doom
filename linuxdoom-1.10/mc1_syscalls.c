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

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// TODO(m): This should be implemented by newlib (?!).
int mkdir (const char *pathname, mode_t mode)
{
  (void)pathname;
  (void)mode;
  errno = EACCES;
  return -1;
}

// TODO(m): Emulate access(), perhaps using stat()? We probably only have to
// deal with mode == R_OK.
int access(const char *pathname, int mode)
{
  (void)pathname;
  (void)mode;
  errno = EACCES;
  return 0;
}

char _mrisc32_inbyte (void)
{
  return '?';
}

int _mrisc32_outbyte (char x)
{
  return 0;
}

