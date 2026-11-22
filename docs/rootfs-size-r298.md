# r298 根文件系统软件盘点

2026-11-21。现行配方见 [共用软件清单](../fetch/rootfs-packages.json)。以下候选尚未应用。

## 两个基线

原归档 `debian12-y4-base-20261107.tar.zst` 的 SHA256 为 `da7aa675629194323109338a83f21fcfe6ee51a8217275867b2ae981844299ec`。从中读取 `./var/lib/dpkg/status`，筛选 `Status: install ok installed`，得到 1,350 个包。另有两个仅残留配置的条目。Y4 r298 发布文件树的同一数据库有 1,406 个包，即 1,405 个公共包和一个板级包。按包名比对，1,010 个版本相同、327 个版本发生变化、13 个已经移除、69 个新增。非 dpkg 管理的厂商文件和面板 ELF 另按输入清单校验。

旧包包含 GNOME、Qt WebEngine、WebKitGTK、Fcitx5、Docker CE、Samba、NFS、OpenSSH、Python 和 Perl。成品另装 Chromium。旧的 6.1 内核、调试符号、头文件及 glmark2、mlucas 已清出 r298。单是旧内核调试包的 dpkg Installed-Size 就有 1,528.7 MiB。

## 整包候选

在 r298 独立文件树中运行 `apt-get -s remove debian-keyring libvulkan-dev aptitude aptitude-common`。APT 计划只移除点名的四包。按普通文件 inode 排除其它路径仍在使用的硬链接，四包共占 67,292,130 字节，即 64.17 MiB。实际 full 和 OTA 的压缩收益须在同条件下制作两版并比较。

| 包 | 文件 MiB | 使用影响 |
| --- | ---: | --- |
| debian-keyring | 30.5 | 本机核验 Debian 开发者签名需要这个包。APT 仓库签名使用 debian-archive-keyring。 |
| libvulkan-dev | 20.2 | 本机编译 Vulkan 程序需要开发头文件和链接。图形运行保留 libvulkan1 和厂商驱动。 |
| aptitude、aptitude-common | 13.5 | 管理员可使用 apt 和 apt-get，需要 aptitude 命令或交互界面时另装。 |

## Y4 r298 实机回归（2026-11-21）

COM5 上的 Y4 在原系统槽运行，按“记录基线 → APT 实际移除四包 → 回归 → 本地安装原版本 DEB → 再次回归”完成两轮。APT 只移除了点名的四包，每次有 67,291,737 字节的独占普通文件消失，约 64.17 MiB。该数字不包含系统日志、目录或文件系统元数据。未重做全量或 OTA 压缩包。

- 四包缺席时，APT 依赖检查通过，从现用镜像下载的 Debian bookworm 和 bookworm-security InRelease 均由保留的 debian-archive-keyring 验签通过。
- GNOME 虚拟屏仍可用。新建的 EGL/GLES 上下文使用 Mali-G720-Immortalis。Chromium 执行 JavaScript 并输出预期页面，Fcitx5 在独立会话将 nihao 提交为“你好”。相同 H.264 测试码流经硬件解码后，卸包前、卸包后、恢复后的 30 帧哈希逐帧一致。面板首页返回 HTTP 200，板载 API、Docker Compose 和两个既有容器均正常。
- 标准 Vulkan 加载器在三个阶段都报告找不到驱动（vkCreateInstance 返回 −9）。r294 与 r298 的 Y4 正式文件树均缺少系统级 Mali Vulkan ICD 清单。仅为测试进程指定临时清单、指向出厂已有的 /opt/cixgpu-pro/lib/libmali.so.0 后，三个阶段均可枚举 Mali-G720-Immortalis、创建逻辑设备并取得图形队列。该测试使用原始 r298 文件树。
- 最终装回原版本，1,406 个已安装包及全部包版本、APT 自动/手动和 hold 标记均与基线一致。全程沿用同一启动，六项观测服务重启计数为零，systemd 无失败单元。这里验证了运行中卸包与恢复，未验证精简后的全新 rootfs 首启、升级或 Y12 真机。

### 原始 r298 交付后的 Vulkan 修复

两板共用的[系统 ICD 清单](../overlay/usr/share/vulkan/icd.d/mali_icd.aarch64.json)指向冻结的 `libmali.so.0.54.1` 的 SONAME，文件权限为 0644。该驱动直接报告 Vulkan 1.4.305。正式构建在图形安装和最终文件树验收时检查清单、版本、库路径和读取权限。

在 COM5 的 Y4 r298 A 槽单独安装了该清单。清除 Vulkan 驱动选择环境变量后，root 和非 root 用户均发现 Mali-G720-Immortalis，创建逻辑设备和队列，完成 4,096 字节的 GPU 填充、同步和主机逐字读回。Mali EGL 和原有系统服务正常。本次只更新了测试机正在运行的 A 槽，已交付的 r298 压缩包保持原版本。Y12 共用相同 SHA256 的驱动，实机 Vulkan 验收待设备到位。

## 语言与桌面

若产品交付范围确定为中英文，保留 C、英语及各中文变体后，可以逐文件评估其他语言的 gettext 消息（326.1 MiB）、GNOME 帮助（71.6 MiB）、Chromium 资源（46.5 MiB）和 Qt WebEngine 资源（18.3 MiB），合计约 462.5 MiB。数据来自 r298 正式根目录，统计独占普通文件，已有硬链接去重的部分不重复计数。此前构建已共享约 89.1 MiB 的 changelog 和 Chromium 索引。验证语言规则时应覆盖换语言、中文输入和 APT 升级。

整包撤掉 Qt WebEngine 会连带移除 Fcitx5 拼音。撤掉 WebKitGTK 会牵连 GDM、GNOME Shell 和控制中心。GNOME 是默认桌面，但独立 X11 会话的 x-session-manager 仍指向 startxfce4，x-window-manager 指向 xfwm4。模拟移除 xfce 分类的 16 个包会移除 25 包，约 29.36 MiB，并改变 startx 的备用路径。面板应用市场依赖 Docker Compose，Docker Buildx 是否保留要按客户是否本机构建镜像决定。

后续若采用候选，先调整共用锁定清单，再对两板运行依赖预检与桌面、输入法、图形、多媒体、面板和升级回归。这次盘点及下述实机对照均保留了正式构建的锁定清单。
