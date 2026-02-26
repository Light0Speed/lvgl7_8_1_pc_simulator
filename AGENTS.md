# AGENTS.md

## Cursor Cloud specific instructions

### 项目概览
本仓库是 **LVGL PC Simulator**（`lv_sim_eclipse_sdl`），使用 SDL2 在桌面端模拟嵌入式 LVGL GUI。

### 仓库准备
- 首次进入仓库时先确保子模块完整：
  ```
  git submodule update --init --recursive
  ```
- 关键子模块：`lvgl`、`lv_drivers`、`lv_examples`。

### 构建（必须使用 GCC）
- **必须使用 GCC**，不要使用默认 `cc`（通常是 `clang`）。Makefile 含 GCC 专用告警参数（如 `-Wmaybe-uninitialized`、`-Wclobbered`、`-Wstack-usage`），clang 不支持。
- 由于 LVGL v7.x 代码在 GCC 13 下与 `-Werror` 组合会触发更严格告警，推荐统一使用以下构建命令：
  ```
  make CC=gcc CFLAGS="-O3 -g0 -I/workspace/ -Wall -Wno-unused-function -Wno-unused-parameter -Wno-missing-field-initializers -Wno-cast-qual -Wno-sign-compare -Wno-missing-prototypes -Wno-old-style-declaration -Wno-cast-function-type" -j$(nproc)
  ```
- 产物二进制为：`./demo`。

### 运行（图形环境与无头环境）
- 模拟器需要 X11 显示。
- 无头环境推荐：
  ```
  Xvfb :1 -screen 0 1024x768x24 &
  DISPLAY=:1 ./demo
  ```
- 应用启动后窗口标题应为 **TFT Simulator**，并持续运行 LVGL 事件循环。

### 变更后的最小验证流程
1. 使用上面的 GCC 命令构建，确认 `./demo` 可生成。
2. 在无头环境用 `Xvfb` 启动并运行 `./demo`，确认程序能正常进入事件循环（无立即崩溃）。
3. 若改动涉及 UI 或输入事件，需进行一次手工交互验证（鼠标/键盘）并记录证据（截图或视频）。

### Lint / 静态检查
- 顶层工程无独立 linter；以 GCC 编译告警为主要质量门禁（见 `Makefile` 中 `WARNINGS`）。
- 可选静态检查：`lvgl/scripts/cppcheck_run.sh`（需安装 `cppcheck`）。

### 测试说明
- 顶层模拟器工程无内建自动化测试套件。
- `lvgl/tests/` 下的测试（Makefile + `build.py`）主要验证 LVGL 库本身，不等同于模拟器端到端验证。

### 系统依赖
- 常用依赖：`build-essential`、`libsdl2-dev`、`xvfb`。

### 常见问题
- 使用 clang 构建失败：改用 `CC=gcc`。
- GCC 13 下告警导致失败：使用本文给出的 `CFLAGS` 覆盖告警选项。
- 程序无法启动显示：检查 `DISPLAY` 是否正确以及 `Xvfb` 是否已启动。
