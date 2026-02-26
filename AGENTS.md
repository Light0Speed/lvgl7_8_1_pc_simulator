# AGENTS.md

## Cursor Cloud specific instructions

### Project overview
This is the **LVGL PC Simulator** (`lv_sim_eclipse_sdl`) — a C application that uses SDL2 to simulate an embedded LVGL GUI on desktop. See `README.md` for general usage.

### Build
- **Must use GCC** (not the default `cc` which is `clang`). The Makefile uses GCC-specific warning flags (`-Wmaybe-uninitialized`, `-Wclobbered`, `-Wstack-usage`, etc.) that are unsupported by clang.
- The Makefile's `-Werror` flag causes build failures with GCC 13 due to stricter warnings on older LVGL v7.x code. Override warnings to build:
  ```
  make CC=gcc CFLAGS="-O3 -g0 -I/workspace/ -Wall -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers -Wno-cast-qual -Wno-sign-compare -Wno-missing-prototypes -Wno-old-style-declaration -Wno-cast-function-type" -j$(nproc)
  ```
- Output binary: `./demo`

### Running
- The simulator requires an X11 display. In headless environments use Xvfb:
  ```
  Xvfb :1 -screen 0 1024x768x24 &
  DISPLAY=:1 ./demo
  ```
- The app opens a window titled "TFT Simulator" and runs an LVGL event loop.

### Lint / Static analysis
- No dedicated linter configured. GCC warnings serve as the lint layer — see the `WARNINGS` variable in `Makefile`.
- LVGL upstream includes `lvgl/scripts/cppcheck_run.sh` for optional static analysis (requires `cppcheck`).

### Tests
- No automated test suite in the top-level simulator project.
- LVGL upstream has tests in `lvgl/tests/` (Makefile + `build.py`), but these test the library in isolation, not the simulator.

### Dependencies (system-level)
- `build-essential`, `libsdl2-dev`, `xvfb` (for headless runs).
- Git submodules (`lvgl`, `lv_drivers`, `lv_examples`) must be initialized.
