
/*
   Copyright (c) 2014 Malte Hildingsson, malte (at) afterwi.se

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

struct timebase {
	unsigned long freq;
	unsigned numer;
	unsigned denom;
};

void timebase_init(struct timebase *tb);

unsigned long timebase_count(void);
unsigned long timebase_msec(struct timebase *tb, unsigned long count);

struct timer {
	unsigned long count;
	double period;
	float scale;
	float raw_delta;
	float smooth_delta;
};

void timer_init(struct timer *t, struct timebase *tb);
void timer_update(struct timer *t);

void snooze(unsigned msec);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* AW_TIME_H */

