# ────────────────────────────────
# This makefile is just for running
# repetitive commands.
# Could instead be a justfile.
# .

MACHINE_GICv2 := virt
MACHINE_GICv3 := virt,gic-version=3

MACHINE_FLAGS := $(MACHINE_GICv3)

CPU_FLAGS := cortex-a76

# ────────────────────────────────
# Utils
# .
dump-dtb:
	@echo -e "[DTB] Dump QEMU device-tree-blob"

	@qemu-system-aarch64 \
		-m 256M \
		-cpu $(CPU_FLAGS) \
		-machine $(MACHINE_FLAGS),dumpdtb=dtb-out.dtb

	@dtc dtb-out.dtb -o dtb-out.txt
	@rm dtb-out.dtb
	@echo "[DTB] human readable dtb dumped to dtb-out.txt"

help-qemu:
	@echo -e "[HELP]\n"
	@qemu-system-aarch64 \
		-m 256M \
		-cpu $(CPU_FLAGS) \
		-machine $(MACHINE_FLAGS),help

nix-repl:
	@echo -e "[NIX] Opening nix repl \n"
	@nix repl -f '<nixpkgs>' -I nixpkgs=channel:nixos-25.05

# ────────────────────────────────
# qemu booting
# .

TARGET := hello_includeos.elf.bin

kernel:
	@echo -e "[RUN] Launching QEMU with '-kernel flag'\n"
	qemu-system-aarch64 \
		-m 2G \
		-machine $(MACHINE_FLAGS) \
		-cpu $(CPU_FLAGS) \
		-nographic \
		-kernel $(TARGET) \

uboot:
	@echo -e "[RUN] Launching QEMU (w. uboot as bios)\n"
	@echo -e "[GUIDE] Wait until uboot SHELL opens, then"
	@echo -e "[GUIDE] use the 'go' cmd to start application"
	@echo -e "[GUIDE] found at address of the <_start> label \n"
	aarch64-unknown-linux-musl-objdump -d hello_includeos.elf.bin | head | tail -n 5
	qemu-system-aarch64 \
		-m 2G \
		-machine $(MACHINE_FLAGS) \
		-cpu $(CPU_FLAGS) \
		-nographic \
		-kernel $(TARGET) \
		-bios u-boot.bin \
		| tee uboot.log

image-boot: $(TARGET)
	@aarch64-unknown-linux-musl-objcopy -O binary $(TARGET) includeos.img
	@qemu-system-aarch64 \
		-m 2G \
		-machine $(MACHINE_FLAGS) \
		-cpu $(CPU_FLAGS) \
		-nographic \
		-kernel includeos.img \
		-bios u-boot.bin

# ────────────────────────────────
# qemu debug booting
# .

debug:
	@echo -e "[DGB] Launching QEMU with '-kernel flag' AND waiting for debugger\n"
	qemu-system-aarch64 \
		-s -S \
		-m 2G \
		-machine $(MACHINE_FLAGS) \
		-cpu $(CPU_FLAGS) \
		-nographic \
		-kernel $(TARGET) \
		-d int,mmu -D qemu.log

# ────────────────────────────────
# debug attach
# .

gdb-attach:
	@echo -e "[DGB] attaching gdb to qemu instance\n"
	gdb $(TARGET) -ex "target extended-remote :1234" -ex "break _start"

NIX_BUILD_ROOT  := /build/source
PROJECT_ROOT    := /home/ee/workspace/IncludeOS-dir/tmp-stuff/AArch64-IncludeOS
SUBSTITUTE_ROOT := set substitute-path $(NIX_BUILD_ROOT) $(PROJECT_ROOT)

gef-attatch:
	@echo -e "[DGB] attaching gdb to qemu instance\n"
	gdb $(TARGET) -ex "$(SUBSTITUTE_ROOT)" -ex "gef-remote --qemu-user localhost 1234" -ex "break _start"


# ────────────────────────────────
# raspberry pi
# .

pi-kernel:
	sudo qemu-system-aarch64 -enable-kvm \
		-m 2G \
		-machine virt,accel=kvm,gic-version=host -cpu host -nographic \
		-kernel $(TARGET)

pi-debug:
	sudo qemu-system-aarch64 \
		-m 2G \
		-machine virt,accel=kvm,gic-version=host -cpu host -nographic \
		-kernel $(TARGET) \
		-s -S

pi-gef-attatch:
	gdb $(TARGET) -ex "$(SUBSTITUTE_ROOT)" -ex "gef-remote --qemu-user 10.42.0.133 1234" -ex "break _start"

