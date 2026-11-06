#ifndef BOARD_VALIDATE_H
#define BOARD_VALIDATE_H
#include <stdint.h>
#include <stdio.h>

/* Synchronous adapter: copy input into board-accessible memory, maintain caches,
 * start hardware, enforce a timeout, then copy outputs back. Return zero on
 * success and fill elapsed_ns with the documented measurement interval.
 * The harness pointers are ordinary CPU arrays, not DMA buffers. */
typedef int (*board_run_fn)(void *context, const int16_t *input,
                            int32_t *output, unsigned n, uint64_t *elapsed_ns);
int board_validate(board_run_fn run, void *context, FILE *csv);
#endif
