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

// Emulate access() using stat()?
int access (const char *pathname, int mode)
{
  struct stat buf;
  if (stat (pathname, &buf) != 0)
  {
    errno = EACCES;
    return -1;
  }

  // We just say: If the file exists, we're good to go.
  return 0;
}

