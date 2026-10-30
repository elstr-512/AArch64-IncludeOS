# AARCH64 INCLUDEOS

- The current target is: `QEMU Arm System emulator - ‘virt’ generic virtual
platform`
- The aarch64 source files in this build are in: `<project root>/src/platform/aarch64_qemu_virt`

## Build aarch64 Includeos
Default unikernel build, enters a nix-shell
```sh
nix-shell develop-arm-aarch64.nix
```

Build output is placed into `./out-includeos`, with:
- Makefile. Not for making, for repetitive commands
- u-boot

Available arguments for: ccache, use a different shell
```sh
nix-shell develop-arm-aarch64.nix --arg withCcache true --arg useZsh true
```

Build encoder service
```sh
nix-shell develop-compare-kernels-encoder.nix
```

