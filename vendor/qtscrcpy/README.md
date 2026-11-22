# QtScrcpy for MYT Edge

The Y4 and Y12 root filesystems ship the ARM64 QtScrcpy build in
`opt/y4-qtscrcpy/`. The product overlay provides desktop entries and the
application icon. The root filesystem builder links the application's
local `adb` path to Debian's `/usr/bin/adb` and installs the matching USB udev
rules from the shared package manifest.

## Provenance

| Component | Source |
| --- | --- |
| QtScrcpy | [barry-ran/QtScrcpy](https://github.com/barry-ran/QtScrcpy), commit `b018bec` |
| QtScrcpyCore | [barry-ran/QtScrcpyCore](https://github.com/barry-ran/QtScrcpyCore), commit `9e388b8a` |
| Build environment | Debian 12 ARM64 and system GCC |
| FFmpeg ABI | Debian libavcodec 59 with V4L2 M2M codecs |
| Product patch | [y4-qtscrcpy-core.patch](y4-qtscrcpy-core.patch), applied inside QtScrcpyCore |

The patch builds against Debian's FFmpeg and tries the `h264_v4l2m2m`
hardware decoder first. It falls back to the native decoder when the
hardware decoder cannot be opened. Set `QTSCRCPY_SW_DECODE=1` to select the
software decoder for comparison. The stock renderer handles the planar
YUV420P frames returned by the VPU.

## Rebuild and validate

Use Debian 12 ARM64. Check out the recorded QtScrcpy and QtScrcpyCore
revisions for an exact-source rebuild. [The build recipe](build.sh) shows
the package dependencies, patch application and CMake build steps.

The rootfs payload contains `QtScrcpy` and `scrcpy-server` under
`/opt/y4-qtscrcpy/`. After replacing either binary, update the corresponding
file sizes and SHA256 values in the [rootfs package manifest](../../fetch/rootfs-packages.json).
Review desktop launch, USB discovery, mirroring and VPU decoding on the
target board. The [APT input list](../../fetch/inputs-apt.json) records the
Qt and FFmpeg runtime dependencies.
