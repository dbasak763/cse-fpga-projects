# Environment

## Host

A C99 compiler and Make are sufficient:

```sh
make test
make sanitize
```

The code uses `<stdint.h>` and standard C library facilities. There are no Python dependencies or C++ source files. Host checks run on Apple Clang locally and GCC in GitHub CI.

## HLS

The Tcl flow is intended for Vitis HLS 2024.2 and Vivado IP export. Source the settings script for your vendor installation and record the versions with each run. Vendor execution has not been verified in this environment.

```sh
make -C fir/Baseline report
CLOCK_NS=8 FPGA_PART=xc7z020-clg400-1 make -C fir/Unroll4 report
```

Each invocation resets its own generated project under `build/hls/<variant>`. Archive reports before repeating a variant with a different clock/part. Run variants sequentially.

## Board

Integrate the exported IP into a board-specific Vivado design and use its generated C driver from a Vitis standalone application. The portable validation harness under `project_files/board/` defines the adapter contract; the board-specific launch/cache/timer adapter remains to be implemented and tested on hardware.
