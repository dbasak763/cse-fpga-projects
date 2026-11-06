CC ?= cc
CFLAGS ?= -std=c99 -O2 -Wall -Wextra -Wpedantic -Wno-unused-label
SOURCES = fir/common/decimate.c fir/tests/decimate_test.c

.PHONY: test sanitize clean
test: build/decimate_test
	./build/decimate_test

build/decimate_test: $(SOURCES) fir/common/decimate.h
	mkdir -p build
	$(CC) $(CFLAGS) -Ifir/common $(SOURCES) -o $@

sanitize:
	mkdir -p build
	$(CC) -std=c99 -O1 -g -Wall -Wextra -Wpedantic -Wno-unused-label -fsanitize=address,undefined -Ifir/common $(SOURCES) -o build/decimate_sanitize
	./build/decimate_sanitize

clean:
	rm -rf build
