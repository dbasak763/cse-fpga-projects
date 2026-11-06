CC ?= cc
CFLAGS ?= -std=c99 -O2 -Wall -Wextra -Wpedantic -Wno-unused-label
SOURCES = fir/common/decimate.c fir/tests/decimate_test.c

.PHONY: test sanitize clean
test: build/decimate_test build/board_harness_test
	./build/decimate_test
	./build/board_harness_test

build/decimate_test: $(SOURCES) fir/common/decimate.h
	mkdir -p build
	$(CC) $(CFLAGS) -Ifir/common $(SOURCES) -o $@

build/board_harness_test: fir/common/decimate.c fir/common/decimate.h project_files/board/validate.c project_files/board/validate.h fir/tests/board_harness_test.c
	mkdir -p build
	$(CC) $(CFLAGS) -Ifir/common -Iproject_files/board fir/common/decimate.c project_files/board/validate.c fir/tests/board_harness_test.c -o $@

sanitize:
	mkdir -p build
	$(CC) -std=c99 -O1 -g -Wall -Wextra -Wpedantic -Wno-unused-label -fsanitize=address,undefined -Ifir/common $(SOURCES) -o build/decimate_sanitize
	./build/decimate_sanitize
	$(CC) -std=c99 -O1 -g -Wno-unused-label -fsanitize=address,undefined -Ifir/common -Iproject_files/board fir/common/decimate.c project_files/board/validate.c fir/tests/board_harness_test.c -o build/board_sanitize
	./build/board_sanitize

clean:
	rm -rf build
