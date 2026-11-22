# MYT Edge Debian 12 ARM64 根文件系统

本仓库维护 MYT Edge Y4 和 Y12 共用的 Debian 软件包清单、桌面与输入法设置、系统配置和厂商运行库。公开和内部构建都从同一个 GitHub `main` 固定源码提交。整机构建和板载服务由 [myt-edge](https://github.com/opencecs/myt-edge) 维护，内核、驱动和设备树由 [cix-kernel](https://github.com/opencecs/cix-kernel) 维护。

[支持的硬件](https://github.com/opencecs/myt-edge/blob/main/docs/PLATFORM-OVERVIEW.md)和[整机开发手册](https://github.com/opencecs/myt-edge/blob/main/docs/DEVELOPMENT-HANDOFF.md)说明两种板型的适用场景、各仓库分工和跨仓库验证流程。

## 正式构建的输入

| 文件或目录 | 用途 |
| --- | --- |
| [base.json](base.json) | 指定 `debootstrap` 和共用软件包清单的 SHA256 |
| [fetch/rootfs-packages.json](fetch/rootfs-packages.json) | 锁定两板共用 Debian 软件包的版本、架构、下载地址、大小和 SHA256 |
| [fetch/inputs-apt.json](fetch/inputs-apt.json) | 记录产品显式依赖、APT 仓库和冻结的软件包输入 |
| [fetch/panel.json](fetch/panel.json) | 记录默认面板 ELF 的源码提交、版本、构建配置、大小和 SHA256 |
| [Y4 内核输入](fetch/inputs-kernel.json)、[Y12 内核输入](fetch/inputs-kernel-y12.json) | 分别锁定两板的内核 Image、DTB 和模块归档 |
| [overlay](overlay)、[vendor](vendor) | 产品配置和 CIX GPU、VPU、片上 NPU、音频 DSP、QtScrcpy 等适配内容 |

构建从空目录执行 `debootstrap`，并按清单安装软件包。MYT Edge 通过 `--board Y4` 或 `--board Y12` 选择对应的内核、设备树、板载服务和 MCU 制品。构建机可通过 `MYT_ROOTFS_DEB_CACHE` 复用通过校验的 DEB，使用 `--fresh-apt` 可用空下载缓存验证构建。Debian bookworm 和 bookworm-security 的镜像配置以 APT 输入清单为准，构建和运行时保留仓库签名校验。

预装面板使用 ARM64 的局域网配置 `nofrp`，实际版本和校验值见 [面板清单](fetch/panel.json)。MYT Edge 校验面板 ELF 并将它安装到 `/usr/bin/myt_desktop`。面板源码由独立项目维护。

## 构建与交付

从 MYT Edge 组装固定版本的三个源码仓库，按板型构建，并显式指定 `YYYYMMDD-rNN` 格式的正式系统版本。公开构建使用发布方提供的已签名 MCU 制品、维护程序和面板 ELF，并核对相应清单和摘要。MCU 固件编译及签名由内部流程完成，签名私钥保存在内部签名环境。[双板构建手册](https://github.com/opencecs/myt-edge/blob/main/docs/DUAL-BOARD-BUILD.md)

- **Fastboot 全量包**含根文件系统、启动分区和磁盘分区表。构建时提供对应板型的官方 `myt-spi.bin`，由它承载 DDR、CSU PM 和 UEFI。
- **系统 OTA**使用同一正式系统版本的 rootfs、内核和 DTB，配套模块随 rootfs 交付。安装要求设备信任系统 OTA 的签名，并接受目标板型与版本。SPI 固件通过 Fastboot 单独更新。根文件系统中已签名的 MCU 制品由板载服务按 MCU 自身的校验流程处理。

全量 ZIP 和 OTA ZIP 的制作、校验及安装步骤以 MYT Edge 的手册为准。[刷机与 OTA 手册](https://github.com/opencecs/myt-edge/blob/main/docs/BUILD-AND-FLASH-GUIDE.md)

## 维护与验证

1. 获取 GitHub `main`，核对工作区的已有改动。
2. 修改系统配置时检查 overlay 文件的最终路径和权限。修改 APT 依赖时同时检查产品依赖清单、完整锁定清单和 `base.json` 记录的摘要。用 `--fresh-apt` 验证空缓存构建。
3. 更新面板制品时核对 ARM64 架构、构建配置、版本、大小和摘要。对用户空间依赖进行精简时，检查 APT 依赖并回归桌面、输入法、多媒体、面板和容器服务。
4. 从三个固定的 GitHub 源码提交组装新构建，检查镜像中的版本、驱动和软件包。验收需要覆盖正常关机重启、关键服务和对应板型。涉及持久化数据时，检查 `/data` 的挂载和迁移。

两板镜像都包含 RK182X 宿主驱动。外接卡使用的模型、用户态运行库及端点升级工具由业务容器提供。[QtScrcpy 说明](vendor/qtscrcpy/README.md)记录投屏程序来源、ADB 集成和 VPU 解码配置。

### A/B 槽与管理地址

每个新系统槽首启时生成自己的 SSH 主机密钥。切槽后，`bond10g` 的运行 MAC 和 DHCP 租约也可能改变，使用串口核对设备地址与 SSH 指纹。同机切槽时，网卡的永久 MAC 和 `/data` 分区标识保持一致。Fastboot 全量重装时，数据保留取决于目标盘是否已有兼容分区布局。

### Mali Vulkan

两板共用 CP6187 的 Mali-G720-Immortalis 用户态库。[系统 Vulkan ICD 清单](overlay/usr/share/vulkan/icd.d/mali_icd.aarch64.json)指向 `/opt/cixgpu-pro/lib/libmali.so.0`。更新库时核对清单的 `api_version` 与驱动实际报告的 Vulkan 版本。验收时清除临时驱动选择环境变量，以普通用户验证 GPU 枚举、队列提交和缓冲区读回。桌面 EGL 使用独立的 GLVND 清单。

### 常开策略

[logind 配置](overlay/etc/systemd/logind.conf.d/90-y4-always-on.conf)和[睡眠配置](overlay/etc/systemd/sleep.conf.d/90-y4-always-on.conf)禁用主机自动休眠。设备级运行时电源管理、温控和风扇遵循对应驱动和板型策略。
