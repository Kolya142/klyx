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

typedef long ptrdiff_t;

typedef long ssize_t;
typedef long pid_t;
typedef long uid_t;
typedef long gid_t;
typedef long off_t;
typedef unsigned long size_t;
typedef unsigned long time_t; // Always 64-bit

typedef unsigned char sig_t; // Anyways POSIX has only 28 signals.
