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
#include <stdint.h>

// MINIX!!1!

typedef struct mfs_super_block {
    // Real/HD part
    
    uint16_t inodes;
    uint16_t zones;
    uint16_t imap_blocks;
    uint16_t zmap_blocks;
    uint16_t firstdatazone;
    uint16_t log_zone_size;
    uint32_t max_size;
    uint16_t magic;
    uint16_t state;

    // Memory part
    bool rd_only;
    bool dirt;
} mfs_super_block_t;

typedef struct mfs_dir_entry {
    uint16_t inode;
    char name[];
} mfs_dir_entry_t;

typedef struct mfs_inode {
    uint16_t mode;
    uint16_t uid;
    uint32_t size;
    uint32_t time;
    uint8_t gid;
    uint8_t nlinks;
    uint16_t zone[9];

    // Memory part
    bool dirt;
} mfs_inode_t;


// This is oct, not dec
#define MFS_S_IFSOCK 0140000
#define MFS_S_IFLNK 0120000 // Symbolic Linker
#define MFS_S_IFREG 0100000
#define MFS_S_IFBLK 0060000
#define MFS_S_IFDIR 0040000
#define MFS_S_IFCHR 0020000
#define MFS_S_IFIFO 0010000
#define MFS_S_ISUID 0004000
#define MFS_S_ISGID 0002000
#define MFS_S_ISVTX 0001000 // WTF?
#define MFS_S_IRUSR 00400
#define MFS_S_IWUSR 00200
#define MFS_S_IXUSR 00100
#define MFS_S_IRGRP 00040
#define MFS_S_IWGRP 00020
#define MFS_S_IXGRP 00010
#define MFS_S_IROTH 00004
#define MFS_S_IWOTH 00002
#define MFS_S_IXOTH 00001
