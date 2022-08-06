{ stdenv, version, src, lib, boost, gtest
, cmake
, ninja
, enableShared ? true
, enableStatic ? !enableShared
}:

stdenv.mkDerivation {
  pname = "grasp-mining";
  inherit version;
  inherit src;

  nativeBuildInputs = [ cmake ninja ];
  buildInputs = [ boost gtest ];

  # Remove warnings during debug builds
  hardeningDisable = ["fortify"];
  cmakeFlags = [
    "-DCMAKE_BUILD_TYPE=Release"
    (lib.optional enableStatic "-DBUILD_STATIC=1")
    (lib.optional (!enableStatic) "-DENABLE_TESTS=1")
  ];
  enableParallelBuilding = true;
  enableParallelChecking = true;
  doCheck = false;
}
