#include "decimate.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

static uint32_t state = 23017;
static unsigned checks;
static uint32_t random_word(void) {
    state ^= state << 13;
    state ^= state >> 17;
    state ^= state << 5;
    return state;
}

/* Generate Pascal coefficients independently of the kernel table. */
static int32_t reference(const int16_t *x, unsigned m) {
    int64_t sum = 0, choose = 1;
    const int end = (int)(4 * m + 3);
    for (int k = 0; k <= 16; ++k) {
        if (end >= k) sum += choose * x[end - k];
        if (k < 16) choose = choose * (16 - k) / (k + 1);
    }
    return (int32_t)(sum / 65536);
}

static void check(const int16_t *x, unsigned n) {
    const int32_t guard = 123456789;
    int32_t out[MAX_SAMPLES / DECIMATION + 2];
    const unsigned count = n <= MAX_SAMPLES ? n / DECIMATION : 0;
    for (unsigned i = 0; i < sizeof out / sizeof out[0]; ++i) out[i] = guard;
    decimate(x, out + 1, n);
    if (out[0] != guard) exit(2);
    for (unsigned i = 0; i < count; ++i) {
        const int32_t expected = reference(x, i);
        if (out[i + 1] != expected) {
            fprintf(stderr, "n=%u output=%u: got %" PRId32 " expected %" PRId32 "\n",
                    n, i, out[i + 1], expected);
            exit(1);
        }
    }
    for (unsigned i = count + 1; i < sizeof out / sizeof out[0]; ++i)
        if (out[i] != guard) exit(3);
    ++checks;
}

int main(void) {
    int16_t samples[MAX_SAMPLES + 1];
    for (unsigned n = 0; n <= 96; ++n) {
        for (unsigned i = 0; i < n; ++i)
            samples[i] = (int16_t)((int32_t)(random_word() % 65536u) - 32768);
        check(samples, n);
    }
    const unsigned lengths[] = {127, 1024, 4095, 4096, 4097};
    for (unsigned t = 0; t < sizeof lengths / sizeof lengths[0]; ++t) {
        const unsigned n = lengths[t];
        for (unsigned i = 0; i < n; ++i)
            samples[i] = (int16_t)((int32_t)(random_word() % 65536u) - 32768);
        check(samples, n);
    }
    const int values[] = {0, 32767, -32768};
    for (unsigned t = 0; t < sizeof values / sizeof values[0]; ++t) {
        int32_t out[MAX_SAMPLES / 4];
        for (unsigned i = 0; i < MAX_SAMPLES; ++i) samples[i] = (int16_t)values[t];
        check(samples, MAX_SAMPLES);
        decimate(samples, out, MAX_SAMPLES);
        if (out[4] != values[t]) return 4;
    }
    for (unsigned i = 0; i < 128; ++i) samples[i] = 0;
    samples[0] = 32767;
    check(samples, 128);
    for (unsigned i = 0; i < 128; ++i) samples[i] = (i & 1) ? -32768 : 32767;
    check(samples, 128);
    for (unsigned i = 0; i < 128; ++i) samples[i] = 0;
    check(samples, 128);
    decimate(NULL, NULL, 0);
    decimate(NULL, NULL, MAX_SAMPLES + 1);
    printf("PASS: %u blocks; reference, bounds, and null/no-op checks\n", checks);
    return 0;
}
