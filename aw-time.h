
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

#ifdef __cplusplus
extern "C" {
#endif

typedef signed char time_s8_t;
typedef unsigned char time_u8_t;
typedef signed short time_s16_t;
typedef unsigned short time_u16_t;
typedef signed int time_s32_t;
typedef unsigned int time_u32_t;
#if _MSC_VER
typedef signed __int64 time_s64_t;
typedef unsigned __int64 time_u64_t;
#else
typedef signed long long time_s64_t;
typedef unsigned long long time_u64_t;
#endif
typedef float time_f32_t;
typedef double time_f64_t;

struct timebase {
	time_u64_t freq;
	time_f64_t inv_freq;
	time_u32_t numer;
	time_u32_t denom;
	time_u32_t period;
};

void timebase_initialize(struct timebase *tb);
void timebase_terminate(struct timebase *tb);

time_u64_t timebase_count(void);
time_u64_t timebase_msec(const struct timebase *tb, time_u64_t count);

struct timer {
	time_u64_t count;
	time_f32_t scale;
	time_f32_t raw_delta;
	time_f32_t smooth_delta;
};

void timer_initialize(struct timer *t, const struct timebase *tb);
void timer_update(struct timer *t, const struct timebase *tb);

void snooze(time_u32_t msec);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_TIME_H */

