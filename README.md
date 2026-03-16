# openGl playground

## Build presets

Configure and build with debug symbols:

```bash
cmake --preset debug
cmake --build --preset debug
./build/debug/src/prog
```

Configure and build with AddressSanitizer + UBSan:

```bash
cmake --preset asan
cmake --build --preset asan
./build/asan/src/prog
```

Quick non-interactive crash trace for the debug build:

```bash
gdb -q -batch -ex run -ex "bt full" --args ./build/debug/src/prog
```

## Milestones

https://github.com/user-attachments/assets/fee36a86-c893-455d-bec3-ae67aecdc8df



