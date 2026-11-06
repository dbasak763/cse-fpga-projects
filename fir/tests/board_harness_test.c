#include "decimate.h"
#include "validate.h"
#include <stdio.h>

static int software(void *context, const int16_t *input, int32_t *output,
                    unsigned n, uint64_t *elapsed_ns) {
    (void)context;
    decimate(input, output, n);
    *elapsed_ns = 0; /* Deliberately no hardware timing in this host test. */
    return 0;
}
static int corrupt(void *context, const int16_t *input, int32_t *output,
                   unsigned n, uint64_t *elapsed_ns) {
    software(context, input, output, n, elapsed_ns);
    output[0] ^= 1;
    return 0;
}
static int failed(void *context, const int16_t *input, int32_t *output,
                  unsigned n, uint64_t *elapsed_ns) {
    (void)context; (void)input; (void)output; (void)n; (void)elapsed_ns;
    return 7;
}
int main(void) {
    FILE *sink = tmpfile();
    if (sink == NULL) return 1;
    if (board_validate(software, NULL, sink) != 0) return 2;
    if (board_validate(corrupt, NULL, sink) != -2) return 3;
    if (board_validate(failed, NULL, sink) != 7) return 4;
    if (board_validate(NULL, NULL, sink) != -1) return 5;
    fclose(sink);
    puts("PASS: board harness with software, corrupt, and failed adapters (no hardware measurements)");
    return 0;
}
