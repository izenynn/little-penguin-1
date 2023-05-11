# clangd

To generate a `compile_commands.json` just run:
```bash
bear -- make
```

For some reason `build` seems to remove the `compile_commands.json` file on the `clean` rule, so rename it before calling `clean`, and later rename it back to `compile_commands.json`

#
