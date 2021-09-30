{ stdenv, lib, boost, cmake, gtest, static ? false }:

stdenv.mkDerivation {
  name = "grasp-mining";
  version = "0.0";
  src = ./.;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ boost ];
  checkInputs = [ gtest ];
  cmakeFlags = [
    (lib.optional static "-DBUILD_STATIC=1")
    (lib.optional (!static) "-DENABLE_TESTS=1")
  ];
  makeTarget = "grasp";
  enableParallelBuilding = true;
  doCheck = true;
  checkTarget = "full_test";
  installPhase = ''
    mkdir -p $out/bin;
    cp grasp $out/bin/;
  '';
}
