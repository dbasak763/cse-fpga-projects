# What I want to measure

Start with the same input length, target part, and clock for all three variants. Change one directive set at a time. The goal is to understand when extra parallelism helps, rather than to assume an unroll factor translates directly into speedup.

| Experiment | Hypothesis | Evidence needed |
| --- | --- | --- |
| Serial to pipelined tap loop | Scheduling may reduce loop overhead | Achieved loop II and recurrence warnings |
| Four-way unroll | More read ports may help feed the multipliers | Banking report, LUT/FF/DSP/BRAM, achieved II |
| 64 / 256 / 1024 / 4096 input samples | Control/DDR overhead may dominate small blocks | Repeated board timings with a documented timer and warmup |
| 10 ns versus 8 ns requested clock | Tighter timing may increase scheduling pressure | Synthesis estimate, then routed timing |

Use `FPGA_PART` and `CLOCK_NS` to override defaults. Start with `xc7z020-clg400-1` at 10 ns. Do not compare reports with different targets as if only the directives changed.

`fir/results.csv` intentionally starts with only a header. Copy actual synthesis report values into the table and retain their source report paths. Do not replace unknown values with guesses. Top-level interval fields refer to kernel invocations, not the tap-loop II. Look at the loop report for the requested inner-loop II. Dynamic block sizes can leave the top-level latency unknown.

Synthesis timing is an estimate. A passing host test is arithmetic evidence. C/RTL co-simulation is functional RTL evidence. Routed timing and measurements on a board provide different evidence; keep them separate.

## Current validation

- Host C99 test: 108 blocks passed, including boundary and guard checks.
- AddressSanitizer and UndefinedBehaviorSanitizer: the same 108 blocks passed.
- Synthesis, C/RTL co-simulation, implementation, and board execution: pending; vendor tools and a connected board are unavailable in the authoring environment.

## Follow-up problems

1. Retain history between blocks and test decimation phase across arbitrary chunk boundaries.
2. Replace the binomial filter with a low-pass designed for an explicit passband and alias-rejection requirement.
3. Compare direct-form filtering against a polyphase organization while preserving bit-exact rounding.
4. Stream blocks instead of buffering the whole input; measure the resulting memory and interface tradeoffs.
