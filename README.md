# Dev_Demo
Is's begin of the new account.
## linux 内核 ARM64 编译流程（基于 Ubuntu 虚拟机）
本项目记录了如何从下载官方 Linux 内核源码开始，到在 ARM 架构的 Ubuntu 虚拟机中成功完成编译的全过程。

---

###  第一步：下载源码（在宿主机执行）
#### 方法一 通过 wget 下载
```bash
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.6.84.tar.xz
```
#### 方法二 直接网页下载
![kernel.org](./image/kernel.org.png)
https://web.git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/refs/tags
###  第二步：通过 SCP 发送到虚拟机
```bash
scp linux-6.6.84.tar.xz username@虚拟机IP:/home/username
```

### 第三步：在虚拟机中解压源码
```bash
cd ~
tar -xjf linux-6.6.84.tar.xz
cd linux-6.6.84
```

### 第四步：安装 ARM 架构交叉编译工具和依赖
```bash
sudo apt update
sudo apt install build-essential gcc-aarch64-linux-gnu \
libncurses-dev bison flex libssl-dev libelf-dev
```
### 第五步：使用默认配置（defconfig）
```bash
make defconfig
```
### 第六步：开始编译
```bash
make -j$(nproc)
```
---

## author
kehong chen
## Date
2025-3-21
