/* vim: set ts=4 sw=4 noet : */
/*
   Copyright (c) 2014-2021 Malte Hildingsson, malte (at) afterwi.se

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
 */

#ifndef _time_nofeatures
# if __linux__
#  define _BSD_SOURCE 1
#  define _DEFAULT_SOURCE 1
#  define _POSIX_C_SOURCE 200809L
#  define _SVID_SOURCE 1
# elif __APPLE__
#  define _DARWIN_C_SOURCE 1
# endif
#endif /* _time_nofeatures */

#include "aw-time.h"

#if _WIN32
# include <windows.h>
#elif __CELLOS_LV2__
# include <sys/sys_time.h>
# include <sys/timer.h>
#elif __APPLE__ || __linux__
# if __APPLE__
#  include <mach/mach_time.h>
# endif
# include <time.h>
#endif
#include <stdlib.h>

void timebase_initialize(struct timebase *tb) {
#if __APPLE__
	mach_timebase_info_data_t info;
	mach_timebase_info(&info);

	tb->freq = 1000000000;
	tb->inv_freq = 1. / (double) tb->freq;
	tb->numer = info.numer;
	tb->denom = info.denom;
	tb->period = 1;
#elif _WIN32
	TIMECAPS caps;
	uint32_t period;

	timeGetDevCaps(&caps, sizeof caps);
	period = caps.wPeriodMin > 1 ? caps.wPeriodMin : 1;
	period = caps.wPeriodMax < period ? caps.wPeriodMax : period;
	timeBeginPeriod(period);

	QueryPerformanceFrequency((LARGE_INTEGER *) &tb->freq);
	tb->inv_freq = 1. / (double) tb->freq;
	tb->numer = 1;
	tb->denom = 1;
	tb->period = period;
#elif __CELLOS_LV2__
	tb->freq = sys_time_get_timebase_frequency();
	tb->inv_freq = 1. / (double) tb->freq;
	tb->numer = 1;
	tb->denom = 1;
	tb->period = 1;
#elif __linux__
	tb->freq = 1000000000;
	tb->inv_freq = 1. / (double) tb->freq;
	tb->numer = 1;
	tb->denom = 1;
	tb->period = 1;
#endif
}

void timebase_terminate(struct timebase *tb) {
#if _WIN32
	timeEndPeriod(tb->period);
#endif
}

uint64_t timebase_count(void) {
#if __APPLE__
	return mach_absolute_time();
#elif _WIN32
	LARGE_INTEGER n;
	QueryPerformanceCounter(&n);
	return n.QuadPart;
#elif __CELLOS_LV2__
	return __mftb();
#elif __linux__
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts.tv_sec * 1000000000 + ts.tv_nsec;
#endif
}

uint64_t timebase_msec(const struct timebase *tb, uint64_t count) {
	const lldiv_t d = lldiv(count * tb->numer, tb->freq * tb->denom);
	return (d.quot * 1000ull) + (d.rem * 1000ull) / (tb->freq * tb->denom);
}

uint64_t timebase_usec(const struct timebase *tb, uint64_t count) {
	const lldiv_t d = lldiv(count * tb->numer, tb->freq * tb->denom);
	return (d.quot * 1000000ull) + (d.rem * 1000000ull) / (tb->freq * tb->denom);
}

uint64_t timebase_nsec(const struct timebase *tb, uint64_t count) {
	const lldiv_t d = lldiv(count * tb->numer, tb->freq * tb->denom);
	return (d.quot * 1000000000ull) + (d.rem * 1000000000ull) / (tb->freq * tb->denom);
}

void timer_initialize(struct timer *t, const struct timebase *tb) {
	t->count = timebase_count();
	t->scale = 1.f;
	t->raw_delta = 0.f;
	t->smooth_delta = 0.f;
}

void timer_update(struct timer *t, const struct timebase *tb) {
	uint64_t count = timebase_count();
	double weight = 1. / 3.;
	double delta = (double) (count - t->count) * tb->inv_freq * t->scale;

	t->count = count;
	t->raw_delta = (float) delta;
	t->smooth_delta = (float) (weight * delta + (1. - weight) * t->smooth_delta);
}

void snooze(uint32_t msec) {
#if _WIN32
	Sleep(msec);
#elif __CELLOS_LV2__
	sys_timer_usleep(msec * 1000);
#elif __linux__ || __APPLE__
	uint32_t sec = msec / 1000;
	struct timespec ts;

	ts.tv_sec = sec;
	ts.tv_nsec = (msec - sec * 1000) * 1000000;

	while (nanosleep(&ts, &ts) == -1)
		;
#endif
}

