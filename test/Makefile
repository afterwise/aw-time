
export CFLAGS += -std=c99 -D_DEFAULT_SOURCE -D_POSIX_C_SOURCE=200809L -Wall -Wextra

ifeq ($(shell uname -s),Linux)
export LDFLAGS += -lrt
endif

test: test.o ../libaw-time.a
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I.. -c $< -o $@

../libaw-time.a:
	$(MAKE) -C..

.PHONY: clean
clean:
	rm -f test test.o

