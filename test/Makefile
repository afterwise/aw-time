
export CFLAGS += -std=c99 -D_DEFAULT_SOURCE -Wall -Wextra

test: test.o ../libaw-time.a
	$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I.. -c $< -o $@

../libaw-time.a:
	$(MAKE) -C..

.PHONY: clean
clean:
	rm -f test test.o

