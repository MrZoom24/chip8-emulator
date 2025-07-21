#include <iostream>
#include "chip8.h"
#include "display.h"
#include <SDL2/SDL.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " path_to_rom.ch8" << std::endl;
        return 1;
    }

    Chip8 chip8;
    chip8.initialize();

    if (!chip8.loadROM(argv[1])) {
        std::cerr << "Failed to load ROM: " << argv[1] << std::endl;
        return 1;
    }

    initDisplay();

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        // Poll SDL events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // TODO: handle keyboard input here
        }

        chip8.emulateCycle();

        if (chip8.drawFlag) {
            updateDisplay(chip8.gfx);
            chip8.drawFlag = false;
        }

        SDL_Delay(2); // Small delay to reduce CPU usage
    }

    destroyDisplay();
    return 0;
}
