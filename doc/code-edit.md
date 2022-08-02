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

# File formatting
Call the following function in the command bar:
```
lua vim.lsp.buf.formatting()
```
