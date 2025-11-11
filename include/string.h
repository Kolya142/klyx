//    Klyx - an operating system kernel.
//    Copyright (C) 2025 Nikolay Shevelko
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, see <https://www.gnu.org/licenses/>.



#pragma once
#include <stddef.h>

static inline void *memcpy(void *restrict s1, void *restrict s2, size_t n) {
    // WHY, INLINE ASSSEMBLY? WHY?
    /*
    asm volatile(
	"cld\n"
	"rep\n"
	"movsb"
	::"c"(n),"S"(s2),"D"(s1)
	);
    */
    unsigned char *si1 = s1;
    unsigned char *si2 = s2;
    for (;n;--n,++si1,++si2) {
	*si1 = *si2;
    }
    return s1;
}

static inline void *memset(void *s, int c, size_t n) {
    /*
    asm volatile(
	"cld\n"
	"rep\n"
	"stosb"
	::"a"(c),"D"(s),"c"(n)
	);
    */
    unsigned char *si = s;
    for (;n;--n,++si) {
	*si = c;
    }
    return s;
}

static inline char *strcpy(char *restrict s1, char *restrict s2) {
    char *s = s1;
    for (;*s2;++s1,++s2) {
	*s1 = *s2;
    };
    return s;
}

static inline int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s1==*s2) {
	++s1;
	++s2;
    }
    return *s1-*s2;
}
