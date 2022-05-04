{ lib, stdenv, boost, gtest
, cmake
, enableRelease ? true
, enableDebug ? false
, enableShared ? true
, enableStatic ? !enableShared
, ...
}:

stdenv.mkDerivation {
  name = "grasp-mining";
  version = "0.0";
  src = ./.;
  nativeBuildInputs = [ cmake ];
  buildInputs = [ boost ];
  checkInputs = [ gtest ];
  cmakeFlags = [
    (lib.optional enableStatic "-DBUILD_STATIC=1")
    (lib.optional (!enableStatic) "-DENABLE_TESTS=1")
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
