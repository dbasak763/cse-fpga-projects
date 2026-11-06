# CSE FPGA Projects

I was interested in building small HLS practice problems alongside coursework and exploring how C maps onto an FPGA: write a kernel, verify the arithmetic, then vary scheduling and memory organization.

This personal practice project starts with a **17-tap integer low-pass filter followed by 4:1 decimation** for a Zynq-7020 target. The kernel and test bench are written in **C99**. HLS automation uses Tcl and Make; there are no Python sources or dependencies.

## Layout

| Folder | Contents |
| --- | --- |
| `fir/Baseline/` | Serial control implementation directives |
| `fir/Pipeline/` | Pipelined tap and load loop directives |
| `fir/Unroll4/` | Four-way tap unrolling and memory banking |
| `fir/common/` | Shared C kernel and header |
| `fir/tests/` | C reference and boundary tests |
| `fir/scripts/` | HLS Tcl runner |
| `project_files/board/` | Portable C board-validation harness |
| `docs/` | Experiment plan and board integration |
| `ENVIRONMENT.md` | Build instructions |

## Host checks

```sh
make test
make sanitize
```

The test bench covers 108 random, DC, impulse, and alternating-sign blocks, plus guard writes and null/no-op behavior. No vendor libraries are needed for host checks.

## HLS experiments

```sh
make -C fir/Baseline report
make -C fir/Pipeline report
make -C fir/Unroll4 cosim
```

The default target is `xc7z020-clg400-1` at a requested 10 ns clock. Review the actual reports under `build/hls/<variant>/solution/syn/report/` and record results in `fir/results.csv`.

Host arithmetic checks pass. Synthesis, C/RTL co-simulation, implementation, and board measurements are pending: the authoring environment has no Vitis/Vivado tools or attached board. A requested clock or II is not a measured result.

See [the filter contract](fir/README.md), [experiment plan](docs/experiments.md), and [board integration](docs/board.md). This binomial filter is an arithmetic/scheduling exercise; a continuous signal chain needs explicit state, phase handling, and a designed anti-alias specification.
