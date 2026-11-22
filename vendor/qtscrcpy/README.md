# QtScrcpy (Y4 build)

Android screen mirroring, patched to decode the scrcpy H.264 stream on
the Y4 VPU instead of the CPU. The binaries in `opt/y4-qtscrcpy/` are
the shipped payload; `overlay/`-side files (desktop entries, icon) live
in the usual overlay paths of this repository.

## Provenance

| Piece | Value |
|---|---|
| App repo | https://github.com/barry-ran/QtScrcpy `b018bec` (main, 2026-11) |
| Core submodule | https://github.com/barry-ran/QtScrcpyCore `9e388b8a` |
| Build host | Y4 board (10.10.20.54), Debian 12 arm64, system gcc |
| FFmpeg | system Debian 5.1.9 (libavcodec 59) - carries the v4l2m2m codecs |
| Patch | `y4-qtscrcpy-core.patch` (2 files, +37/-6, applied inside QtScrcpyCore) |

## What the patch changes

1. `QtScrcpyCore/CMakeLists.txt` (Linux branch): build against the
   system ffmpeg. The vendored headers are ffmpeg 4.x while Debian ships
   libavcodec 59, and the vendored lib directory ships no arm64
   binaries. The v4l2m2m hardware codecs come from the system ffmpeg.
2. `QtScrcpyCore/src/device/decoder/decoder.cpp` `open()`: prefer
   `h264_v4l2m2m` (the VPU decoder), fall back to the native software
   decoder when it is absent or cannot open, honor the
   `QTSCRCPY_SW_DECODE=1` escape hatch. No renderer change is needed:
   the mvx V4L2 driver delivers planar YUV420P capture buffers
   (verified on the board: "requesting formats: output=H264
   capture=YU12"), so the stock three-plane YUV420P OpenGL shader
   keeps working unchanged.

## Reproduce

Run `build.sh` on any Debian 12 arm64 machine (the board itself is the
reference). Install `QtScrcpy` and `scrcpy-server` into `/opt/y4-qtscrcpy/`.
The application resolves `adb` next to the executable. The rootfs builder
links that path to the Debian ARM64 `/usr/bin/adb`, installed from the common
APT manifest with the matching USB udev rules.

## Runtime dependencies (image)

`libqt5gui5 libqt5widgets5 libqt5network5 libqt5multimedia5
libqt5multimedia5-plugins libqt5x11extras5` plus the stock ffmpeg
libav* set that the image already ships. Listed in
`rootfs/fetch/inputs-apt.json` with this delivery.

## Verification status (2026-11-13)

- Cross-build on the Y4 board: clean, links system libavcodec 59.
- Launch in the live GNOME wayland session (xcb/XWayland): starts,
  enumerates `adb devices`, no crash.
- Hardware-decode streaming: needs a phone attached to the board;
  planned as part of the r194 delivery verification with the user.
