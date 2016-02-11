
#include "aw-time.h"

#include <assert.h>
#include <stdio.h>

/* XXX: ci debugging */
#if __linux__
# if _POSIX_C_SOURCE - 0 < 1L
#  warning "no _POSIX_C_SOURCE"
# elif _POSIX_C_SOURCE - 0 < 199309L
#  warning "insufficient _POSIX_C_SOURCE"
# endif
#endif

int main(int argc, char *argv[]) {
	(void) argc;
	(void) argv;

	struct timebase tb;
	struct timer tim;
	unsigned long tick, tock;
	double len;

	timebase_init(&tb);

	tick = timebase_count();
	snooze(2000);
	tock = timebase_count();
	len = timebase_msec(&tb, tock - tick);
	printf("snoozed 2000ms => %f\n", len);
	assert(len > 1000.0);
	assert(len < 3000.0);

	timer_init(&tim, &tb);
	len = 0.0;
	for (int i = 0; i < 200; ++i) {
		snooze(10);
		timer_update(&tim);
		len += tim.smooth_delta;
	}
	printf("snoozed 200*10ms => %f\n", len);
	assert(len > 1.0);
	assert(len < 3.0);

	printf("OK\n");
	return 0;
}

