# CHIP-8 Emulator

A simple CHIP-8 emulator written in C++ using SDL2.  
Currently supports graphics, input, and core instruction set functionality.

---

## Features

- ✅ Basic CHIP-8 emulation (most instructions implemented)
- ✅ Graphics rendering with SDL2
- ✅ Keyboard input support (mapped to CHIP-8 keypad)
- 🚧 No sound yet
- 🚧 No ROM file picker (manual ROM selection required)
- 🚧 No Windows support yet

---

## Building

### Requirements
- C++17 compiler
- SDL2

### Example build command (Linux)
```bash
g++ main.cpp chip8.cpp display.cpp -o chip8 -lSDL2
```

## Controls
The CHIP-8 keypad is mapped like this:
1 2 3 C      →      1 2 3 4
4 5 6 D      →      Q W E R
7 8 9 E      →      A S D F
A 0 B F      →      Z X C V

## Usage
For now, the ROM path must be hardcoded or passed directly in code.
Execution from terminal after build:
```bash
./chip8 <path_to_rom>
```
