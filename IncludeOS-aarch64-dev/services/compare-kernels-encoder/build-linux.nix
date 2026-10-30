{
  nixpkgs ? ../../../pinned.nix

, target ? "x86_64"

}:

let
  # cross compile configured nix packages
  supportedTargets.pkgs = {
    x86_64 = import nixpkgs {
      crossSystem = { config = "x86_64-unknown-linux-musl"; };
    };

    aarch64 = import nixpkgs {
      crossSystem = { config = "aarch64-unknown-linux-musl"; };
    };
  };

  # Select pkgs configuration
  pkgs = supportedTargets.pkgs.${target};
in

pkgs.llvmPackages_20.stdenv.mkDerivation {
  pname = "hello_linux";
  version = "0.1.0";

  src = ./.;

  nativeBuildInputs = [
    pkgs.buildPackages.cmake
  ];

  buildInputs = [
    pkgs.pkgsMusl.pkgsStatic.x264
  ];

  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
  ];
}
