# Vulkan

Build the tests:

```cmd
cmake --build build\windows --target unit_tests --config Debug
```

Run the tests:

```cmd
ctest --test-dir build\windows -C Debug --output-on-failure
```