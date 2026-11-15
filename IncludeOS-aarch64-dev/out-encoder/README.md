# Video Encoding Benchmark

The file `develop-compare-kernels-encoder.nix`, buildt the three
benchmark configuration:
- includeos_x264_encoder.elf.bin
- linux_x264_native.out
- linux_VM

All three executables expects [the tractor video sequense](https://media.xiph.org/video/derf/)
as a video file located at:
```
./videos-yuv/tractor.yuv
```

### includeos_x264_encoder.elf.bin
This can be run fully virtualized with the following command, this is SLOW:
```
qemu-system-aarch64 \
        -m 4G \
        -cpu cortex-a76 \
        -machine virt,gic-version=3 \
        -kernel includeos_x264_encoder.elf.bin \
        -device loader,file=./videos-yuv/tractor.yuv,addr=0x80000000 \
        -nographic
```
##### QEMU/KVM

To run it using QEMU/KVM, you will need to use an AArch64 platform.
Using the following command:
```
qemu-system-aarch64 \
        -enable-kvm \
        -smp 1 \
        -m 4G \
        -cpu host \
        -machine virt,accel=kvm,gic-version=host \
        -kernel includeos_x264_encoder.elf.bin \
        -device loader,file=./videos-yuv/tractor.yuv,addr=0x80000000 \
        -nographic
```

### linux_x264_native.out

This can be run on an AArch64 platform with Linux. With the following command:
```
./linux_x264_native.out
```

### linux_VM

This requires the [Linux kernel](https://kernel.org/) compiled for AArch64, and a
[Mini root filesystem](https://alpinelinux.org/downloads/) for AArch64.

At the root of the Mini root filesystem, place the `linux_VM` executable,
with an `init` script for launching it.

```
bin/
dev/
etc/
home/
lib/
media/
mnt/
opt/
proc/
root/
run/
sbin/
srv/
sys/
tmp/
usr/
var/
init
linux_VM
```
Where init contains:
```
#!/bin/busybox sh

mount -t devtmpfs  devtmpfs  /dev
mount -t proc      proc      /proc
mount -t sysfs     sysfs     /sys
mount -t tmpfs     tmpfs     /tmp

/linux_VM

sleep 2

poweroff -f

sh
```
- The Linux kernel must be available as `Image`.
- The Mini root filesystem must be compressed to `initramfs.cpio.gz`

