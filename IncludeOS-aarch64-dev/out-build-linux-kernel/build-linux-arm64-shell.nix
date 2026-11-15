{
  pkgs ? import <nixpkgs> {
    crossSystem = {
      config ="aarch64-unknown-linux-gnu";
    };
  }
}:

pkgs.mkShell {

  nativeBuildInputs = with pkgs; [
    bison
    flex
    bc
    perl
    python3
    openssl
    elfutils
  ];


  shellHook = ''
    export ARCH="arm64";
    export CROSS_COMPILE="${pkgs.stdenv.cc.targetPrefix}";
    make defconfig
    make -j$NIX_BUILD_CORES Image
    ll arch/arm64/boot/Image
  '';
}
