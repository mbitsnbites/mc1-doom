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
//      Cross-platform routines for portability.
//
//-----------------------------------------------------------------------------

#include <ctype.h>

#include "m_port.h"

int M_strcmpi (const char* s1, const char* s2)
{
    while (tolower(*s1) == tolower(*s2)) {
        if (*s1 == 0)
            return 0;
        ++s1;
        ++s2;
    }
    return tolower(*s2) - tolower(*s1);
}

int M_strncmpi (const char* s1, const char* s2, size_t n)
{
    if (n == 0)
        return 0;

    size_t i = 0;
    while (i < (n - 1) && tolower(*s1) == tolower(*s2)) {
        if (*s1 == 0)
            return 0;
        ++s1;
        ++s2;
        ++i;
    }
    return tolower(*s2) - tolower(*s1);
}
