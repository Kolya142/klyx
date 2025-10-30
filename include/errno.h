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

// this file implements part of the POSIX.

enum {
    ENO = 0, // No error
    E2BIG, // Argument list too long.
    EACCES, // Permission denied.
    EADDRINUSE, // Address in use.
    EADDRNOTAVAIL, // Address not available.
    EAFNOSUPPORT, // Address family not supported.
    EAGAIN, // Resource unavailable, try again (may be the same value as [EWOULDBLOCK]).
    EALREADY, // Connection already in progress.
    EBADF, // Bad file descriptor.
    EBADMSG, // Bad message.
    EBUSY, // Device or resource busy.
    ECANCELED, // Operation canceled.
    ECHILD, // No child processes.
    ECONNABORTED, // Connection aborted.
    ECONNREFUSED, // Connection refused.
    ECONNRESET, // Connection reset.
    EDEADLK, // Resource deadlock would occur.
    EDESTADDRREQ, // Destination address required.
    EDOM, // Mathematics argument out of domain of function.
    EDQUOT, // Reserved.
    EEXIST, // File exists.
    EFAULT, // Bad address.
    EFBIG, // File too large.
    EHOSTUNREACH, // Host is unreachable.
    EIDRM, // Identifier removed.
    EILSEQ, // Illegal byte sequence.
    EINPROGRESS, // Operation in progress.
    EINTR, // Interrupted function.
    EINVAL, // Invalid argument.
    EIO, // I/O error.
    EISCONN, // Socket is connected.
    EISDIR, // Is a directory.
    ELOOP, // Too many levels of symbolic links.
    EMFILE, // File descriptor value too large.
    EMLINK, // Too many hard links.
    EMSGSIZE, // Message too large.
    EMULTIHOP, // Reserved.
    ENAMETOOLONG, // Filename too long.
    ENETDOWN, // Network is down.
    ENETRESET, // Connection aborted by network.
    ENETUNREACH, // Network unreachable.
    ENFILE, // Too many files open in system.
    ENOBUFS, // No buffer space available.
    ENODEV, // No such device.
    ENOENT, // No such file or directory.
    ENOEXEC, // Executable file format error.
    ENOLCK, // No locks available.
    ENOLINK, // Reserved.
    ENOMEM, // Not enough space.
    ENOMSG, // No message of the desired type.
    ENOPROTOOPT, // Protocol not available.
    ENOSPC, // No space left on device.
    ENOSYS, // Functionality not supported.
    ENOTCONN, // The socket is not connected.
    ENOTDIR, // Not a directory or a symbolic link to a directory.
    ENOTEMPTY, // Directory not empty.
    ENOTRECOVERABLE, // State not recoverable.
    ENOTSOCK, // Not a socket.
    ENOTSUP, // Not supported (may be the same value as [EOPNOTSUPP]).
    ENOTTY, // Inappropriate I/O control operation.
    ENXIO, // No such device or address.
    EOPNOTSUPP, // Operation not supported on socket (may be the same value as [ENOTSUP]).
    EOVERFLOW, // Value too large to be stored in data type.
    EOWNERDEAD, // Previous owner died.
    EPERM, // Operation not permitted.
    EPIPE, // Broken pipe.
    EPROTO, // Protocol error.
    EPROTONOSUPPORT, // Protocol not supported.
    EPROTOTYPE, // Protocol wrong type for socket.
    ERANGE, // Result too large.
    EROFS, // Read-only file system.
    ESOCKTNOSUPPORT, // Socket type not supported.
    ESPIPE, // Invalid seek.
    ESRCH, // No such process.
    ESTALE, // Reserved.
    ETIMEDOUT, // Connection timed out.
    ETXTBSY, // Text file busy.
    EWOULDBLOCK, // Operation would block (may be the same value as [EAGAIN]).
    EXDEV, // Improper hard link.
};
