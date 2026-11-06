#include "validate.h"
#include "decimate.h"
#include <inttypes.h>

static int32_t golden(const int16_t *x, unsigned m) {
    int64_t sum = 0, choose = 1;
    const int end = (int)(4 * m + 3);
    for (int k = 0; k <= 16; ++k) {
        if (end >= k) sum += choose * x[end - k];
        if (k < 16) choose = choose * (16 - k) / (k + 1);
    }
    return (int32_t)(sum / 65536);
}

int board_validate(board_run_fn run, void *context, FILE *csv) {
    int16_t input[MAX_SAMPLES];
    int32_t output[MAX_SAMPLES / DECIMATION];
    const unsigned lengths[] = {64, 256, 1024, 4096};
    uint32_t rng = 23017;
    if (run == NULL || csv == NULL) return -1;
    fprintf(csv, "samples_in,trial,elapsed_ns,correct\n");
    for (unsigned size = 0; size < sizeof lengths / sizeof lengths[0]; ++size) {
        const unsigned n = lengths[size];
        for (unsigned i = 0; i < n; ++i) {
            rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
            input[i] = (int16_t)((int32_t)(rng % 65536u) - 32768);
        }
        for (unsigned trial = 0; trial < 22; ++trial) {
            uint64_t elapsed_ns = 0;
            for (unsigned m = 0; m < n / DECIMATION; ++m) output[m] = 123456789;
            const int status = run(context, input, output, n, &elapsed_ns);
            if (status != 0) return status;
            for (unsigned m = 0; m < n / DECIMATION; ++m)
                if (output[m] != golden(input, m)) return -2;
            /* Validate warmup too, but record only the following 20 trials. */
            if (trial >= 2)
                fprintf(csv, "%u,%u,%" PRIu64 ",1\n", n, trial - 2, elapsed_ns);
        }
    }
    return ferror(csv) ? -3 : 0;
}
