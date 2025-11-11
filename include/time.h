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

struct tm {
    int tm_sec, tm_min, tm_hour; // Day-releated times.
    int tm_mday, tm_mon, tm_year, tm_yday; // Year-releated times.
    int tm_wday; // Weak-releated times.
    int tm_isdst; // Daylight Savings flag.
};

struct timespec {
    time_t tv_sec;
    long tv_nsec;
};

// TODO: <time.h> shall also declare the itimerspec structure


// TODO: CLOCKS_PER_SEC CLOCK_MONOTONIC CLOCK_PROCESS_CPUTIME_ID CLOCK_REALTIME CLOCK_THREAD_CPUTIME_ID TIMER_ABSTIME

/*
  char      *asctime(const struct tm *);
  char      *asctime_r(const struct tm *restrict, char *restrict);
  clock_t    clock(void);
  int        clock_getcpuclockid(pid_t, clockid_t *);
  int        clock_getres(clockid_t, struct timespec *);
  int        clock_gettime(clockid_t, struct timespec *);
  int        clock_nanosleep(clockid_t, int, const struct timespec *,
  struct timespec *);
  int        clock_settime(clockid_t, const struct timespec *);
  char      *ctime(const time_t *);
  char      *ctime_r(const time_t *, char *);
  double     difftime(time_t, time_t);
  struct tm *getdate(const char *);
  struct tm *gmtime(const time_t *);
  struct tm *gmtime_r(const time_t *restrict, struct tm *restrict);
  struct tm *localtime(const time_t *);
  struct tm *localtime_r(const time_t *restrict, struct tm *restrict);
*/

time_t     mktime(struct tm *);

/*
  int        nanosleep(const struct timespec *, struct timespec *);
  size_t     strftime(char *restrict, size_t, const char *restrict,
  const struct tm *restrict);
  size_t     strftime_l(char *restrict, size_t, const char *restrict,
  const struct tm *restrict, locale_t);
  char      *strptime(const char *restrict, const char *restrict,
  struct tm *restrict);
*/

time_t     time(time_t *);

/*
int        timer_create(clockid_t, struct sigevent *restrict,
               timer_t *restrict);
int        timer_delete(timer_t);
int        timer_getoverrun(timer_t);
int        timer_gettime(timer_t, struct itimerspec *);
int        timer_settime(timer_t, int, const struct itimerspec *restrict,
               struct itimerspec *restrict);
void       tzset(void);
*/
