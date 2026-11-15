# AArch64 IncludeOS Services

The current target is: [QEMU Arm System emulator - ‘virt’ generic virtual
platform](https://www.qemu.org/docs/master/system/arm/virt.html)

## Build AArch64 IncludeOS example

The following command sets up a shell and builds the IncludeOS service located
in `./services/example/`.

```sh
nix-shell develop-arm-aarch64.nix
```

The build output is placed into `./out-includeos/`. And the shell is set up with
the following tools and utilities:
- QEMU. For launching the IncludeOS service.
- aarch64-esr-decoder. A utility for decoding the AArch64 Exception Syndrome Register.
- u-boot. An optional boot loader.
- A Makefile. Not for making, for repetitive commands.

Using the command `make kernel`, will execute the following command:

```
qemu-system-aarch64 \
        -m 2G \
        -machine virt,gic-version=3 \
        -cpu cortex-a76 \
        -nographic \
        -kernel hello_includeos.elf.bin \
```

## Build x264 Encoder service

The following command sets up a shell and builds the IncludeOS encoder
benchmark located in `./services/compare-kernels-encoder/`.

```sh
nix-shell develop-compare-kernels-encoder.nix
```

This produces three executables:
- includeos_x264_encoder.elf.bin. The IncludeOS encoder service.
- linux_x264_native.out. ARM aarch64 executable version of the application. Requires an
AArch64 platform.
- linux_VM. The same ARM aarch64 executable as Linux native. Only difference is its output label identifying it as being virtualized.

The build output is placed into `./out-encoder/`. And the shell is set up with
same tools as above, but with a different make file for running the executables.

Instructions for running the executables are listed in `./out-encoder/README.md`

