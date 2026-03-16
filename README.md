# MegaMan64Recompiled

A native PC port of **Mega Man 64** (N64) built with the [N64Recomp](https://github.com/Mr-Wiseguy/N64Recomp) toolchain. The game's MIPS machine code is statically recompiled to C, then compiled as a native executable that runs on Windows, Linux, and macOS — with full controller support, texture pack support, and configurable graphics.

## Features

- Native Windows / Linux / macOS support
- High-resolution rendering via [RT64](https://github.com/rt64/rt64) (Vulkan / D3D12 / Metal)
- Configurable resolution, aspect ratio, anti-aliasing, and refresh rate
- Keyboard + controller support (including gyro and rumble)
- Texture pack support (`.rtz` mod container format)
- In-game settings UI (graphics, controls, audio)
- Quick-save / quick-load (work in progress)

## Requirements

- A legally-obtained NTSC-U Mega Man 64 ROM (`.z64` format)

## Building

See [BUILDING.md](BUILDING.md) for full build instructions.

## License

See [LICENSE](LICENSE) for details.
