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

#ifndef AW_TIME_H
#define AW_TIME_H

#if !_MSC_VER || _MSC_VER >= 1600
# include <stdint.h>
#endif

#if defined(_time_dllexport)
# if _MSC_VER
#  define _time_api extern __declspec(dllexport)
# elif __GNUC__
#  define _time_api __attribute__((visibility("default"))) extern
# endif
#elif defined(_time_dllimport)
# if _MSC_VER
#  define _time_api extern __declspec(dllimport)
# endif
#endif
#ifndef _time_api
# define _time_api extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

struct timebase {
	uint64_t freq;
	double inv_freq;
	uint32_t numer;
	uint32_t denom;
	uint32_t period;
};

_time_api void timebase_initialize(struct timebase *tb);
_time_api void timebase_terminate(struct timebase *tb);

_time_api uint64_t timebase_count(void);
_time_api uint64_t timebase_msec(const struct timebase *tb, uint64_t count);
_time_api uint64_t timebase_usec(const struct timebase *tb, uint64_t count);
_time_api uint64_t timebase_nsec(const struct timebase *tb, uint64_t count);

struct timer {
	uint64_t count;
	float scale;
	float raw_delta;
	float smooth_delta;
};

_time_api void timer_initialize(struct timer *t, const struct timebase *tb);
_time_api void timer_update(struct timer *t, const struct timebase *tb);

_time_api void snooze(uint32_t msec);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_TIME_H */

