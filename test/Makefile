
export CFLAGS += -std=c99 -Wall -Wextra

ifeq ($(shell uname -s),Linux)
export LDLIBS += -lrt
endif

test: test.o ../libaw-time.a
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I.. -c $< -o $@

../libaw-time.a:
	$(MAKE) -C..

.PHONY: clean
clean:
	rm -f test test.o

