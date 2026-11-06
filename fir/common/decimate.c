#include "decimate.h"

void decimate(const int16_t *in_samples, int32_t *out_samples, unsigned n) {
#ifdef __SYNTHESIS__
#pragma HLS INTERFACE m_axi port=in_samples offset=slave bundle=gmem depth=4096
#pragma HLS INTERFACE m_axi port=out_samples offset=slave bundle=gmem depth=1024
#pragma HLS INTERFACE s_axilite port=in_samples bundle=control offset=0x10
#pragma HLS INTERFACE s_axilite port=out_samples bundle=control offset=0x1c
#pragma HLS INTERFACE s_axilite port=n bundle=control offset=0x28
#pragma HLS INTERFACE s_axilite port=return bundle=control
#endif
    const int32_t coefficients[TAPS] = {
        1, 16, 120, 560, 1820, 4368, 8008, 11440, 12870,
        11440, 8008, 4368, 1820, 560, 120, 16, 1
    };
    int16_t samples[MAX_SAMPLES];
    if (n > MAX_SAMPLES) return;
load_loop:
    for (unsigned i = 0; i < n; ++i) samples[i] = in_samples[i];
output_loop:
    for (unsigned m = 0; m < n / DECIMATION; ++m) {
        const unsigned end = DECIMATION * m + DECIMATION - 1;
        int64_t sum = 0;
tap_loop:
        for (unsigned k = 0; k < TAPS; ++k) {
            if (k <= end) sum += (int64_t)samples[end - k] * coefficients[k];
        }
        out_samples[m] = (int32_t)(sum / 65536);
    }
}
