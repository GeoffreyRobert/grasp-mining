# Development shell
```
nix develop
```
You can check that you properly droped in the dev shell using:
```
env | grep NIX
```
If you get a bunch of results, you should be ok.

## Enable direnv
You should automatically drop in the dev shell if you allow it in the repo.
From the repo dir, do:
```
direnv allow
```

# Regular build
```
mkdir build && cd build
cmake -G Ninja ..
cmake --build .
```

# Debug build
```
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
```

# Debug Tests
```
gdb ./full_test
```

# Multi Config
```
cmake . -G"Ninja Multi-Config" -DCMAKE_CONFIGURATION_TYPES="Release;Debug" -DCMAKE_DEFAULT_BUILD_TYPE=Debug -B build/
```
