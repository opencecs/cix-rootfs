#!/bin/sh
# Y4 QtScrcpy build recipe (r194). Run on the Y4 board (Debian 12 arm64).
# Package QtScrcpy and scrcpy-server from CMake; rootfs APT supplies adb.
set -e

DEBIAN_FRONTEND=noninteractive apt-get install -y \
    qtbase5-dev qt5-qmake qtmultimedia5-dev libqt5x11extras5-dev \
    libxcb1-dev libavcodec-dev libavformat-dev libavutil-dev \
    libswscale-dev cmake g++ make git

git clone --depth 1 https://github.com/barry-ran/QtScrcpy.git
cd QtScrcpy

# upstream pins the submodule over SSH; rewrite to HTTPS and fetch
git config submodule.QtScrcpy/QtScrcpyCore.url https://github.com/barry-ran/QtScrcpyCore.git
git submodule update --init --recursive

# Y4 patch: system ffmpeg on Linux + VPU-first decoder
git -C QtScrcpy/QtScrcpyCore apply "$PWD/../y4-qtscrcpy-core.patch" \
    || git -C QtScrcpy/QtScrcpyCore apply "$(dirname "$0")/y4-qtscrcpy-core.patch"

cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j"$(nproc)"

echo "artifacts: $(find build -name QtScrcpy -type f) (+ scrcpy-server alongside; use Debian ARM64 adb)"
