# Board integration in C

The default part is `xc7z020-clg400-1`. Select the preset, constraints, DDR setup, and clock/reset wiring for your actual board; the part number alone is not a complete board design.

1. Export one variant with `make -C fir/Pipeline report`.
2. Add its exported IP from `build/hls/Pipeline/solution/impl/ip` to a board-specific Vivado design with a Zynq Processing System.
3. Connect `s_axi_control` through the PS master port for control. Connect `m_axi_gmem` to a PS high-performance port for DDR access. Wire clocks/reset, assign addresses, validate, implement, and check routed timing.
4. Generate the bitstream and export the hardware platform to a Vitis standalone C application.
5. Use the HLS-exported C driver and generated hardware definitions. Check the generated header for the actual control/register map before calling it.
6. Implement the `board_run_fn` callback from `project_files/board/validate.h` for this platform, then invoke `board_validate` with the adapter and a CSV output stream.

## Adapter contract

The callback receives ordinary CPU arrays. It must stage them into correctly aligned, hardware-accessible buffers, provide those physical addresses to the driver, set `n`, perform cache maintenance for the platform, launch the accelerator, enforce a bounded timeout, and copy completed outputs back. Never pass ordinary stack addresses as DMA physical addresses without establishing that the platform memory mapping and accessibility permit it.

The kernel explicitly requests these AXI-Lite offsets; verify them in the generated driver:

| Offset | Meaning |
| --- | --- |
| `0x00` | Start bit 0, done bit 1, idle bit 2 |
| `0x10`, `0x14` | Input address low/high |
| `0x1c`, `0x20` | Output address low/high |
| `0x28` | Input sample count |

The callback returns elapsed nanoseconds measured with the platform timer. Document whether that interval includes copies, cache maintenance, register setup, and polling. Do not compare differently scoped intervals as kernel speedups. Return nonzero on timeout or driver failure so the harness stops immediately.

The harness tests input lengths 64, 256, 1024, and 4096, validates every output against its C reference, discards two warmup trials per length, and emits twenty measured rows per length. Include the part, board, tool version, actual PL clock, overlay/bitstream identity, and timing scope with recorded data.

Host tests exercise this harness with a software adapter, a deliberately corrupt adapter, and an error-returning adapter. Their temporary output is discarded and is not hardware evidence.

No board-specific adapter, prebuilt bitstream, routed timing, or actual board measurements are provided yet. The portable C harness compiles and is host-tested; hardware integration remains pending.
