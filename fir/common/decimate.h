#ifndef SIGNAL_LAB_DECIMATE_H
#define SIGNAL_LAB_DECIMATE_H
#include <stdint.h>
#define MAX_SAMPLES 4096u
#define DECIMATION 4u
#define TAPS 17u
/* Zero padding per call; writes floor(n/4) values. Oversized blocks do nothing.
 * Input and output must not overlap. */
void decimate(const int16_t *in_samples, int32_t *out_samples, unsigned n);
#endif
