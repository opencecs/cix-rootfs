# MYT Edge Debian 12 ARM64 根文件系统

Y4 和 Y12 从同一个 GitHub `main` 构建。此仓库维护两个板型共用的用户空间、桌面、输入法、应用配置及厂商库。[myt-edge](https://github.com/opencecs/myt-edge) 按 `--board Y4` 或 `--board Y12` 选择内核、DTB、MCU 载荷和板载服务。宿主预置 RK182X 驱动，外接卡的模型、用户态运行库和端点升级工具由业务容器提供。

旧基础包与 r298 的软件包数量、已裁掉的旧内核、可选精简项及依赖影响见 [r298 软件盘点](docs/rootfs-size-r298.md)。

## 正式构建的输入

| 文件 | 用途 |
| --- | --- |
| [base.json](base.json) | 指定 `debootstrap` 和共用软件包清单的 SHA256。 |
| [fetch/rootfs-packages.json](fetch/rootfs-packages.json) | 锁定两板共用的 1,405 个 DEB 的来源、版本、架构和 SHA256，并记录厂商单文件与宿主策略。 |
| [fetch/inputs-apt.json](fetch/inputs-apt.json) | 维护产品显式依赖和 APT 源。变更软件包时同时核对完整锁定清单。 |
| [fetch/panel.json](fetch/panel.json) | 固定预装面板 ELF 的源码提交、版本、构建配置、大小和 SHA256。 |
| [Y4 内核输入](fetch/inputs-kernel.json)、[Y12 内核输入](fetch/inputs-kernel-y12.json) | 各板使用独立的 Image、DTB、模块校验值。 |
| [overlay](overlay)、[vendor](vendor) | 安装产品配置及 CIX GPU、VPU、片上 NPU、音频 DSP 和 QtScrcpy 等适配文件。 |

旧基础包 `base/debian12-y4-base-20261107.tar.zst` 已从当前主线移除。其 dpkg 数据库含 1,350 个已安装包。Y4 正式版 r298 从空目录安装 1,405 个共用包及本板的 `y4-board` 包，共 1,406 个。r298 已在 Y4 实机完成 A、B 两槽启动验收；删包后两种板型的构建输入预检均通过。

QtScrcpy 使用产品适配版本，通过 `/opt/y4-qtscrcpy/adb` 调用 Debian 的 `/usr/bin/adb`。厂商输入由清单校验来源和摘要。

当前预装面板为 `1.1.7-lan-test`，源码 `75af6b6`，两板共用 ARM64 `nofrp` 产物。MYT Edge 从 `--panel-binary` 或本地构建配置读取 ARM64 ELF，按 `fetch/panel.json` 核对并安装到 `/usr/bin/myt_desktop`。

## 构建入口

使用 [MYT Edge](https://github.com/opencecs/myt-edge) 的三仓库组装入口，将本仓库与 [cix-kernel](https://github.com/opencecs/cix-kernel) 固定到所需提交。按 `Y4` 或 `Y12` 构建系统，官方面板和 MCU 制品作为外部输入传入。系统构建与 OTA 打包可省略 UEFI SPI BIN。

完整命令、输入示例和签名说明见[双板构建手册](https://github.com/opencecs/myt-edge/blob/main/docs/DUAL-BOARD-BUILD.md)。

## Mali Vulkan

Y4 和 Y12 共用 CP6187 的 Mali-G720-Immortalis 用户态库。产品 overlay 安装[系统 Vulkan ICD 清单](overlay/usr/share/vulkan/icd.d/mali_icd.aarch64.json)，加载器通过它打开 `/opt/cixgpu-pro/lib/libmali.so.0`。更新 Mali 驱动时应同时核对清单的 `api_version` 与驱动通过 `vk_icdGetInstanceProcAddr` 提供的 `vkEnumerateInstanceVersion`。上板验收时清除驱动选择环境变量，以普通用户验证 GPU 名称、队列提交与缓冲区读回。桌面 EGL 由独立的 GLVND 清单提供。

## 常见开发操作

1. 获取 GitHub `main`，核对工作区已有改动。
2. 修改软件包清单或 overlay 内的文件。更新面板时核对 ARM64 架构、版本、执行权限和服务配置，并用生成的 `panel.json` 固定二进制的摘要。独立项目维护面板源码。
3. 提交已验证的文件。从 MYT Edge 固定三个 GitHub 源码提交并重新组装，构建所需板型。修改 APT 依赖后用 `--fresh-apt` 验证空缓存。新增厂商文件时核对实际安装路径。

### 从空目录制作系统

两板共用 `fetch/rootfs-packages.json`。每项记录软件包、架构、实际安装版本、下载 URL、大小和 SHA256。
`base.json` 固定整份清单的 SHA256。`fetch/inputs-apt.json` 维护产品显式依赖和签名仓库。更新软件时一起核对这两份清单。
构建机需要 debootstrap、dpkg-deb、APT、unshare、qemu-user-static 和 ARM64 binfmt 支持。正式构建会新建文件系统。
可通过 `MYT_ROOTFS_DEB_CACHE` 指定经校验的 DEB 缓存目录。`--fresh-apt` 为本次构建使用独立的空下载缓存。
安装包集合与版本、厂商文件、RK182X 宿主驱动、SSH 首启身份和常开策略都有对应检查。板型决定 DTB、模块、MCU 和板载服务。
RK182X 的模型、用户态运行库及端点刷写工具由业务容器提供。宿主保留驱动和设备节点。主机温控、风扇、关机和重启均由板级配置提供。

### A/B 槽与管理地址

每个新系统槽在首启时生成自己的 SSH 主机密钥。切换系统槽时，bond10g 的运行 MAC 和 DHCP 租约也可能随槽改变。使用串口核对新地址和指纹。三颗网卡的永久 MAC 及 /data 的分区标识在同机切槽时保持一致。

## 验证重点

检查镜像实际文件/权限、服务依赖、应用版本、日志可读性以及正常关机/重启；涉及持久化状态时，核对 /data 数据迁移与绑定挂载，不能把设备业务数据库放进镜像或 Git。

RTC 时间保持还需要核查硬件电池及断电后联网校时前的读数，仅修改时区或开启 NTP 不能证明 RTC 备份供电正常。

系统 OTA 包含 rootfs 和匹配内核。DDR、CSU PM 和 UEFI 由各板型全量包的 `images/myt-spi.bin` 通过 USB Fastboot 更新。标准打包流程由 [myt-edge](https://github.com/opencecs/myt-edge) 维护。[刷写与 OTA 说明](https://github.com/opencecs/myt-edge/blob/main/docs/BUILD-AND-FLASH-GUIDE.md#4a-完整刷机包)

## 国内 Debian 下载源（2026-11-13）

`fetch/inputs-apt.json` 的 Debian bookworm 与 bookworm-security 均使用 HTTPS 中科大镜像；Docker CE 延续现有中科大源。Chromium 三个冻结 DEB 优先国内地址，官方地址保留为后备；版本、架构、大小和 SHA256 不变。构建及安装后的 APT 都保留 Debian 签名与有效期校验，不使用 trusted=yes 或跳过认证。

镜像说明：[USTC Debian](https://mirrors.ustc.edu.cn/help/debian.html)。修改下载清单会正常使旧 APT 缓存失效，不改变已验证内核的三项 pin。


## Y4 / Y12 常开策略

计算设备默认不自动进入整机休眠。overlay 的 90-y4-always-on.conf 禁用空闲、合盖、休眠按键触发，sleep.conf 禁用系统睡眠操作，并屏蔽五个 sleep/suspend/hibernate target。策略随 rootfs 安装，不依赖新守护进程。设备级 runtime PM 与硬件热保护保持原有行为；这不等于强制所有设备永久满频。开发恢复测试可在受控条件下直接使用内核 PM 接口，不能把部分 pm_test 阶段当成完整休眠验收。

## 历史记录：2026-11-15 V11 1.1.3 面板与浏览器修复
- Embedded application: 1.1.3-v11fix-lan-test, panel source 52ac242991b0d5e3b78877234daf0e22003b45cb.
- SHA256: 21c24431ab2335d436d4bf800335f5cf485120384b30f9fe4794f839c8b81e04; Linux ARM64 static ELF. Existing LAN-only/no-FRP build scope retained.
- Y4 system fan controls use the MCU API; default 20%, displayed fans 1/2/3 map to FAN3/4/5.
- Deleted accounts revoke panel/terminal/cast/proxy sessions and same-name SMB credentials/connections. Failed SMB cleanup leaves the account disabled and reports an error.
- User-approved Chromium basic storage avoids keyring prompts; managed policy disables password saving. Existing profile/keyring files are preserved; cookies no longer receive system-keyring protection.
- Source tests, race checks, frontend build, isolated firmware-Samba integration and isolated ARM64 application smoke passed. No physical-board deployment, firmware/OTA build or flash.
- 此历史版本的完整验证报告保存在迁移前的交付归档。旧浏览器和面板会话可能需要重新登录。
