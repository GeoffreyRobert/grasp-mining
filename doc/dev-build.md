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

# ccls Setup
You need to generate a `complile_commands.json` for ccls to be able to index the project.
This file has to be at the root of the project. To generate the file, add
`-DCMAKE_EXPORT_COMPILE_COMMANDS=1` when generating the build with cmake.
Then, you should create a simlink at the root of your project pointing to that file:
```
ln -s ./build/compile_commands.json
```

See `.config/nvim/lua/lsp.lua` for the configured mappings.
See github.com/neovim/nvim-lspconfig#Suggested-configuration for details on the config.

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
