#include "chip8.h"
#include <fstream>
#include <iostream>

const unsigned int FONTSET_SIZE = 80;
const unsigned int START_ADDRESS = 0x200;

uint8_t chip8_fontset[FONTSET_SIZE] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

Chip8::Chip8(){
    initialize();
}

void Chip8::initialize(){
    pc = START_ADDRESS;
    opcode = 0;
    I = 0;
    sp = 0;

    // clear display
    for(int i = 0; i < 64*32; ++i){
        gfx[i] == 0;
    }

    // clear stack, registers and memory
    for(int i = 0; i < 16; ++i){
        stack[i] = 0;
        V[i] = 0;
        keypad[i] = 0;
    }

    for(int i = 0; i < 4096; i++){
        memory[i] = 0;
    }

    // load fontset into memory starting at 0x50
    for(int i = 0; i < FONTSET_SIZE; ++i){
        memory[0x50 + i] = chip8_fontset[i];
    }

    // reset timers
    delay_timer = 0;
    sound_timer = 0;

    drawFlag = false;
}

bool Chip8::loadROM(const std::string& filename){
    std::ifstream rom(filename, std::ios::binary);

    if(!rom.is_open()){
        std::cerr << "Failed to open ROM: " << filename << std::endl;
        return false;
    }


    int index = START_ADDRESS;
    char byte;

    while(rom.get(byte)){
        if(index >= 4096){
            std::cerr << "ROM too large to fit in memory" << std::endl;
            return false;
        }

        memory[index] = static_cast<uint8_t>(byte);
        index++;
    }

    rom.close();
    return true;
}

void Chip8::emulateCycle(){
    opcode = (memory[pc] << 8) | memory[pc + 1];
    pc += 2;

    // Opcodes not yet implemented: 
    //  BNNN, EX9E, EXA1, FX0A

    if((opcode & 0xF000) == 0x0000){
        if((opcode & 0x00F0) == 0x00E0){
            // 00E0: Clear the screen
            for(int i = 0; i < 64*32; ++i){
                gfx[i] = 0;
            }
        }else if((opcode & 0x00FF) == 0x00EE){
            // 00EE: Return from subrutine
            sp--;
            pc = stack[sp];
        }else{
            std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
        }
    }else if((opcode & 0xF000) == 0x1000){
        // 1NNN: Jump to address NNN
        pc = opcode & 0x0FFF;
    }else if((opcode & 0xF000) == 0x2000){
        // 2NNN: Call subroutine at NNN
        stack[sp] = pc;
        sp++;
        pc = opcode & 0x0FFF;
    }else if((opcode & 0xF000) == 0x3000){
        // 3XNN: Skip next instruction if Vx == NN
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x0FF;

        if(V[x] == nn){
            pc += 2;
        }
    }else if((opcode & 0xF000) == 0x4000){
        // 4XNN: Skip next instruction if Vx != NN
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;

        if(V[x] != nn){
            pc += 2;
        }
    }else if((opcode & 0xF000) == 0x5000){
        // 5XY0: Skip next instruction if Vx == Vy
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;

        if(V[x] == V[y]){
            pc += 2;
        }
    }else if((opcode & 0xF000) == 0x6000){
        // 6XNN: Set Vx = NN
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;

        V[x] = nn;
    }else if ((opcode & 0xF000) == 0x7000){
        // 7XNN: Add NN to Vx without carry
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;
        V[x] += nn;

    }else if ((opcode & 0xF000) == 0x8000){
        // 8XYN: Arithmetic and logic operations
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;
        uint8_t n = opcode & 0x000F;

        switch (n){
            case 0x0: 
                // 8XY0: Set Vx = Vy
                V[x] = V[y];
                break;
            case 0x1: 
                // 8XY1: Set Vx = Vx OR Vy
                V[x] |= V[y];
                break;
            case 0x2: 
                // 8XY2: Set Vx = Vx AND Vy
                V[x] &= V[y];
                break;
            case 0x3: 
                // 8XY3: Set Vx = Vx XOR Vy
                V[x] ^= V[y];
                break;
            case 0x4: 
                // 8XY4: Vx += Vy (VF = carry)
                V[0xF] = (V[x] + V[y] > 255) ? 1 : 0;
                V[x] += V[y];
                break;
            case 0x5: 
                // 8XY5: Vx -= Vy (VF = NOT borrow)
                V[0xF] = (V[x] > V[y]) ? 1 : 0;
                V[x] -= V[y];
                break;
            case 0x6: 
                // 8XY6: Vx >>= 1 (VF = LSB)
                V[0xF] = V[x] & 0x1;
                V[x] >>= 1;
                break;
            case 0x7: 
                // 8XY7: Vx = Vy - Vx (VF = NOT borrow)
                V[0xF] = (V[y] > V[x]) ? 1 : 0;
                V[x] = V[y] - V[x];
                break;
            case 0xE: 
                // 8XYE: Vx <<= 1 (VF = MSB)
                V[0xF] = (V[x] & 0x80) >> 7;
                V[x] <<= 1;
                break;
            default:
                std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
                break;
        }
    }else if((opcode & 0xF000) == 0x9000){
        // 9XY0: Skip next instruction if Vx != Vy
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t y = (opcode & 0x00F0) >> 4;

        if(V[x] != V[y]){
            pc += 2;
        }
    }else if((opcode & 0xF000) == 0xA000){
        // ANNN: Set I = NNN
        I = opcode & 0x0FFF;
    }else if((opcode & 0xF000) == 0xB000){
        // BNNN: Jump to address NNN + V0
        pc = (opcode & 0x0FFF) + V[0];
    }else if((opcode & 0xF000) == 0xC000){
        // CXNN: Set Vx = random byte AND NN
        uint8_t x = (opcode & 0x0F00) >> 8;
        uint8_t nn = opcode & 0x00FF;
        V[x] = (rand() % 256) & nn;
    }else if((opcode & 0xF000) == 0xD000){
        // DXYN: Draw sprite 
        // TODO: Actually implement drawing.
        std::cout << "Draw sprite opcode called: " << std::hex << opcode << std::endl;
    }else if ((opcode & 0xF000) == 0xF000){
        uint8_t x = (opcode & 0x0F00) >> 8;
        switch (opcode & 0x00FF) {
            case 0x07: 
                // FX07: Set Vx = delay timer
                V[x] = delay_timer;
                break;
            case 0x15: 
                // FX15: Set delay timer = Vx
                delay_timer = V[x];
                break;
            case 0x18: 
                // FX18: Set sound timer = Vx
                sound_timer = V[x];
                break;
            case 0x1E: 
                // FX1E: I += Vx
                I += V[x];
                break;
            case 0x29: 
                // FX29: Set I to font sprite address for digit in Vx
                I = V[x] * 5;  // each sprite is 5 bytes long
                break;
            case 0x33: 
                // FX33: Store BCD of Vx in memory at I, I+1, I+2
                memory[I] = V[x] / 100;
                memory[I + 1] = (V[x] / 10) % 10;
                memory[I + 2] = V[x] % 10;
                break;
            case 0x55: 
                // FX55: Store V0 to Vx in memory starting at I
                for (int i = 0; i <= x; ++i){
                    memory[I + i] = V[i];
                }
                break;
            case 0x65: 
                // FX65: Load V0 to Vx from memory starting at I
                for (int i = 0; i <= x; ++i)
                    V[i] = memory[I + i];
                break;
            default:
                std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
                break;
        }
    }else{
        std::cout << "Unknown opcode: " << std::hex << opcode << std::endl;
    }

    // Update timers
    if(delay_timer > 0){
        --delay_timer;
    }

    if(sound_timer > 0){
        if(sound_timer == 1){
            // TODO: Actually implement sound
            std::cout << "BEEP!" << std::endl;
        }
        --sound_timer;
    }
};
