{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
      version = builtins.substring 0 0 self.lastModifiedDate;
    in {
      packages.${system}.default = pkgs.callPackage ./derivation.nix {
        src = self;
        inherit version;
        stdenv = pkgs.gccStdenv;
      };
    };
}
