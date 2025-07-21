#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include "chip8.h" 

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 320, SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        64, 32); // Original CHIP-8 resolution

    // Create Chip8 instance
    Chip8 chip8;

    // Test draw: fill checkerboard pattern
    for (int y = 0; y < 32; ++y) {
        for (int x = 0; x < 64; ++x) {
            chip8.gfx[y * 64 + x] = (x + y) % 2;
        }
    }
    chip8.drawFlag = true;

    // Main loop — for now I just draw once
    if (chip8.drawFlag) {
        uint32_t pixels[64 * 32];

        for (int i = 0; i < 64 * 32; ++i) {
            pixels[i] = chip8.gfx[i] ? 0xFFFFFFFF : 0x000000FF; // white or black
        }

        SDL_UpdateTexture(texture, nullptr, pixels, 64 * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
        SDL_RenderPresent(renderer);

        chip8.drawFlag = false;
    }

    // Wait 2 seconds before closing
    SDL_Delay(2000);

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
