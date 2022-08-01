# Development shell
```
nix develop
```

# Regular build
```
mkdir build && cd build
cmake .. -G Ninja
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
