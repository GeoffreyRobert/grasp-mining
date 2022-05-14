{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/21.05";

    utils.url = "github:numtide/flake-utils";
    utils.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, utils }: utils.lib.eachDefaultSystem (system:
    let pkgs = import nixpkgs {
        system = "x86_64-linux";
      };
    in {
      devShell = pkgs.mkShell rec {
        name = "grasp-mining";

        packages = with pkgs; [
          cmake
          gtest
        ];

      defaultPackage = pkgs.callPackage ./derivation.nix {};

      };
    });
}
