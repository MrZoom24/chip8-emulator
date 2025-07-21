#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <string>

class Chip8 {
public: 
    Chip8();

    void initialize();
    bool loadROM(const std::string& filename);
    void emulateCycle();
    void loadTestROM();

    uint8_t gfx[64 * 32];
    uint8_t keypad[16];
    bool drawFlag;

private:
    uint16_t opcode;
    uint8_t memory[4096];
    uint8_t V[16];
    uint16_t I;
    uint16_t pc;
    uint16_t stack[16];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;

    void fetchOpcode();
    void decodeAndExecute();

};
#endif