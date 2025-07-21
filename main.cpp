#include <SDL2/SDL.h>
#include <iostream>
#include "chip8.h"
#include "display.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <path_to_rom>" << std::endl;
        return 1;
    }

    Chip8 chip8;

    if (!chip8.loadROM(argv[1])) {
        std::cerr << "Failed to load ROM." << std::endl;
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    initDisplay();

    SDL_Event event;
    bool quit = false;

    const int CYCLES_PER_FRAME = 10; // Number of emulation cycles per frame (~60 FPS)

    while (!quit) {
        // Handle events (keyboard, quit, etc.)
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_1: chip8.keypad[0x1] = 1; break;
                    case SDLK_2: chip8.keypad[0x2] = 1; break;
                    case SDLK_3: chip8.keypad[0x3] = 1; break;
                    case SDLK_4: chip8.keypad[0xC] = 1; break;

                    case SDLK_q: chip8.keypad[0x4] = 1; break;
                    case SDLK_w: chip8.keypad[0x5] = 1; break;
                    case SDLK_e: chip8.keypad[0x6] = 1; break;
                    case SDLK_r: chip8.keypad[0xD] = 1; break;

                    case SDLK_a: chip8.keypad[0x7] = 1; break;
                    case SDLK_s: chip8.keypad[0x8] = 1; break;
                    case SDLK_d: chip8.keypad[0x9] = 1; break;
                    case SDLK_f: chip8.keypad[0xE] = 1; break;

                    case SDLK_z: chip8.keypad[0xA] = 1; break;
                    case SDLK_x: chip8.keypad[0x0] = 1; break;
                    case SDLK_c: chip8.keypad[0xB] = 1; break;
                    case SDLK_v: chip8.keypad[0xF] = 1; break;
                }
            }
            else if (event.type == SDL_KEYUP) {
                switch(event.key.keysym.sym) {
                    case SDLK_1: chip8.keypad[0x1] = 0; break;
                    case SDLK_2: chip8.keypad[0x2] = 0; break;
                    case SDLK_3: chip8.keypad[0x3] = 0; break;
                    case SDLK_4: chip8.keypad[0xC] = 0; break;

                    case SDLK_q: chip8.keypad[0x4] = 0; break;
                    case SDLK_w: chip8.keypad[0x5] = 0; break;
                    case SDLK_e: chip8.keypad[0x6] = 0; break;
                    case SDLK_r: chip8.keypad[0xD] = 0; break;

                    case SDLK_a: chip8.keypad[0x7] = 0; break;
                    case SDLK_s: chip8.keypad[0x8] = 0; break;
                    case SDLK_d: chip8.keypad[0x9] = 0; break;
                    case SDLK_f: chip8.keypad[0xE] = 0; break;

                    case SDLK_z: chip8.keypad[0xA] = 0; break;
                    case SDLK_x: chip8.keypad[0x0] = 0; break;
                    case SDLK_c: chip8.keypad[0xB] = 0; break;
                    case SDLK_v: chip8.keypad[0xF] = 0; break;
                }
            }
        }

        // Run multiple emulation cycles per frame for speed
        for (int i = 0; i < CYCLES_PER_FRAME; ++i) {
            chip8.emulateCycle();
        }

        if (chip8.drawFlag) {
            updateDisplay(chip8.gfx);
            chip8.drawFlag = false;
        }

        SDL_Delay(16);  // ~60 FPS delay
    }

    destroyDisplay();
    SDL_Quit();

    return 0;
}
