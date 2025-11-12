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
#include <klyx/hw.h>
#include <stdint.h>
#include <time.h>
#include <thirdparty/printf.h>

static int __is_upd(void) {
    outb(0x70, 0x0A);
    return inb(0x71) & 0x80;
}

static uint8_t __get_rtc_reg(int reg) {
    outb(0x70, reg);
    return inb(0x71);
}

static uint8_t __decode_bcd(uint8_t d) {
    return (d>>4)*10+(d&15);
}

#define MINUTE 60
#define HOUR (60*MINUTE)
#define DAY (24*HOUR)
#define YEAR (365*DAY)

static int month[12] = {
    0,
    31,
    31+29,
    31+29+31,
    31+29+31+30,
    31+29+31+30+31,
    31+29+31+30+31+30,
    31+29+31+30+31+30+31,
    31+29+31+30+31+30+31+31,
    31+29+31+30+31+30+31+31+30,
    31+29+31+30+31+30+31+31+30+31,
    31+29+31+30+31+30+31+31+30+31+30
};

// I hate modern search engines that became crazier every f****ing hour and cannot just find POSIX documentation for me.

static void __get_time(struct tm *tm) {
    while (__is_upd());

    // RTC(Legacy)
    tm->tm_sec = __decode_bcd(__get_rtc_reg(0x00));
    tm->tm_min = __decode_bcd(__get_rtc_reg(0x02));
    tm->tm_hour = __decode_bcd(__get_rtc_reg(0x04));
    tm->tm_mday = __decode_bcd(__get_rtc_reg(0x07));
    tm->tm_mon = __decode_bcd(__get_rtc_reg(0x08))-1;
    tm->tm_year = __decode_bcd(__get_rtc_reg(0x09))+2000;

    // 2025-times legacy
    tm->tm_yday = tm->tm_mday+month[tm->tm_mon];
    tm->tm_wday = tm->tm_mday%7;
}

time_t mktime(struct tm *tm) {
    time_t year = tm->tm_year - 1900;
    time_t time = tm->tm_sec + tm->tm_min*60 + tm->tm_hour*3600 + tm->tm_yday*86400 +
	(year-70)*31536000 + ((year-69)/4)*86400 -
	((year-1)/100)*86400 + ((year+299)/400)*86400 - 2*DAY;
    return time;
}

time_t time(time_t *tptr) {
    struct tm tm;
    __get_time(&tm);
    time_t res = mktime(&tm);
    if (tptr) *tptr = res;
    return res;
}
