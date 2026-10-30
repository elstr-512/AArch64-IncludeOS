# ────────────────────────────────
# Variables
# .
MACHINE_GICv2 := virt
MACHINE_GICv3 := virt,gic-version=3
MACHINE_FLAGS := $(MACHINE_GICv3)
CPU_FLAGS := cortex-a76

ENCODER-TARGET := includeos_x264_encoder.elf.bin

# m_LOAD_FILE_VID := ./videos-yuv/foreman.yuv
m_LOAD_FILE_VID := ./videos-yuv/tractor.yuv
m_2_GB_POS  := 0x80000000


# ────────────────────────────────
# Emulated IncludeOS service
# .

includeos-service:
	qemu-system-aarch64 \
		-m 4G \
		-cpu $(CPU_FLAGS) \
		-machine $(MACHINE_FLAGS) \
		-kernel $(ENCODER-TARGET) \
		-device loader,file=$(m_LOAD_FILE_VID),addr=$(m_2_GB_POS) \
		-nographic


# ────────────────────────────────
# Benchmark KVM - Raspberry PI 5
# .

DATE_TOD := 17-03-

run-all-compare:
	sync
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	make run-test-Linux-nat >> ../data-logs/$(DATE_TOD)Linux-NAT

	sync
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	make run-test-IOS >> ../data-logs/$(DATE_TOD)IOS

	sync
	echo 3 | sudo tee /proc/sys/vm/drop_caches
	make run-test-Linux-VM >> ../data-logs/$(DATE_TOD)Linux-VM

run-test-Linux-nat:
	taskset -c 1 ./linux_x264_native.out

run-test-IOS:
	sudo taskset -c 1 qemu-system-aarch64 \
		-enable-kvm \
		-smp 1 \
		-m 4G \
		-cpu host \
		-machine virt,accel=kvm,gic-version=host \
		-kernel $(ENCODER-TARGET) \
		-device loader,file=$(m_LOAD_FILE_VID),addr=$(m_2_GB_POS) \
		-nographic

run-test-Linux-VM:
	sudo taskset -c 1 qemu-system-aarch64 \
		-smp 1 \
		-enable-kvm \
		-cpu host \
		-machine virt,accel=kvm,gic-version=host \
		-m 4G \
		-kernel Image \
		-initrd initramfs.cpio.gz \
		-append "console=ttyAMA0" \
		-drive file=../video.img,if=virtio,format=raw \
		-nographic

