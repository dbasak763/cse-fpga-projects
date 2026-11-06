# Decimation experiment

The common kernel applies the binomial low-pass `(1 + z^-1)^16 / 65536` and keeps samples at indices `3, 7, 11, ...`. This is an integer arithmetic and scheduling exercise, not a filter designed to satisfy a specified anti-alias rejection requirement. Evaluate the response before using it for a real signal chain.

| Variant | Change | Question |
| --- | --- | --- |
| Baseline | Serial loops; automatic pipelining disabled | What does the control design cost? |
| Pipeline | Request II=1 on load and tap loops | Does the accumulation recurrence limit II? |
| Unroll4 | Four-way tap unroll, banked samples, partitioned coefficients | Do extra ports help enough to justify the area? |

Each folder has its own Makefile and directives; all use `common/decimate.c` and the same test bench to keep arithmetic constant. A requested II is not a measured or guaranteed II. The outer output loop is kept serial in every variant, so the inner-loop change is isolated. The 17-tap remainder needs an exit check in the partially unrolled variant.

```sh
make -C fir/Baseline report
make -C fir/Pipeline report
make -C fir/Unroll4 cosim
```

Inspect the generated csynth reports directly and copy measured values into `results.csv`, preserving the source report path.

The HLS project exports an IP catalog package. Integrating it into a board design and producing a bitstream are separate steps described in `docs/board.md`.

## Arithmetic contract

- Input: signed 16-bit samples; up to 4096 per call.
- Output: signed 32-bit integers; exactly `floor(n/4)` values.
- Accumulation: signed 64-bit, followed by division by 65536 toward zero.
- Each call starts with zero history. For continuous recordings, overlap blocks with 16 preceding samples and align the decimation phase yourself, or extend the design with explicit state.
- Incomplete groups of four at the end are dropped. Oversized blocks produce no writes. Input and output buffers must not overlap.
- `n=0` does not dereference either pointer. Callers allocate at least `n` input elements and `floor(n/4)` output elements for valid nonempty calls.

The test bench independently generates Pascal coefficients and checks random blocks, full-scale positive and negative DC, impulse, alternating signs, remainder lengths, oversized input, output guards, and repeated calls. Host sanitizers test memory/undefined behavior; they do not validate RTL timing.
