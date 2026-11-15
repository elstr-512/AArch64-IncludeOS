# AArch64 IncludeOS Archive

This repository contains the AArch64 port of IncludeOS.
Forked from the original IncludeOS found
at: https://github.com/includeos/IncludeOS

## Dependencies
To build IncludeOS you need Nix and Linux.

## Building IncludeOS

To build IncludeOS, use the command `nix-build` to evaluate and build
the expression in `default.nix`. This will build the x86_64 version
of IncludeOS.

```sh
# x86_64 IncludeOS
nix-build default.nix
```

## AArch64 IncludeOS

To build AArch64 IncludeOS, use the build command with the specified
target architecture:

```sh
# AArch64 IncludeOS
nix-build default.nix --argstr target aarch64
```

## Implementation

The current target is: [QEMU Arm System emulator - ‘virt’ generic virtual
platform](https://www.qemu.org/docs/master/system/arm/virt.html)

Demonstrations for building and running AArch64 IncludeOS services,
are located in:
```sh
./IncludeOS-aarch64-dev/
```
