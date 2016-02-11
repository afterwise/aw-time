
/*
   Copyright (c) 2014-2016 Malte Hildingsson, malte (at) afterwi.se

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

#include "aw-time.h"

#if __CELLOS_LV2__
# include <sys/sys_time.h>
# include <sys/timer.h>
#elif __APPLE__ || __linux__
# if __APPLE__
#  include <mach/mach_time.h>
# endif
# include <time.h>
#endif
#include <stdlib.h>

void timebase_init(struct timebase *timebase) {
#if __APPLE__
	mach_timebase_info_data_t info;
	mach_timebase_info(&info);

	timebase->freq = 1000000000;
	timebase->numer = info.numer;
	timebase->denom = info.denom;
#elif _WIN32
	timeBeginPeriod(1);

	QueryPerformanceFrequency((LARGE_INTEGER *) &timebase->freq);
	timebase->numer = 1;
	timebase->denom = 1;
#elif __CELLOS_LV2__
	timebase->freq = sys_time_get_timebase_frequency();
	timebase->numer = 1;
	timebase->denom = 1;
#elif __linux__
	timebase->freq = 1000000000;
	timebase->numer = 1;
	timebase->denom = 1;
#endif
}

unsigned long timebase_count(void) {
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

unsigned long timebase_msec(struct timebase *timebase, unsigned long count) {
	const lldiv_t d = lldiv(count * timebase->numer, timebase->freq * timebase->denom);
	return (d.quot * 1000) + (d.rem * 1000) / (timebase->freq * timebase->denom);
}

void timer_init(struct timer *timer, struct timebase *timebase) {
	timer->count = timebase_count();
	timer->period = 1. / (double) timebase->freq;
	timer->scale = 1.f;
	timer->raw_delta = 0.f;
	timer->smooth_delta = 0.f;
}

void timer_update(struct timer *timer) {
	unsigned long count = timebase_count();
	double weight = 1.0 / 3.0;
	double delta = (double) (count - timer->count) * timer->period * timer->scale;
	double prev_delta = timer->smooth_delta;

	timer->count = count;
	timer->raw_delta = (float) delta;
	timer->smooth_delta = (float) (weight * delta + (1.0 - weight) * prev_delta);
}

void snooze(unsigned msec) {
#if _WIN32
	Sleep(msec);
#elif __CELLOS_LV2__
	sys_timer_usleep(msec * 1000);
#elif __linux__ || __APPLE__
	unsigned sec = msec / 1000;
	struct timespec ts;

	ts.tv_sec = sec;
	ts.tv_nsec = (msec - sec * 1000) * 1000000;

	while (nanosleep(&ts, &ts) == -1)
		;
#endif
}

