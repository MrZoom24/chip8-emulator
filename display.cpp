#include "display.h"
#include <SDL2/SDL.h>
#include <iostream>

const int VIDEO_WIDTH = 64;
const int VIDEO_HEIGHT = 32;
const int SCALE = 10;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

void initDisplay(){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    window = SDL_CreateWindow("CHIP-8 Emulator", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              VIDEO_WIDTH * SCALE, 
                              VIDEO_HEIGHT * SCALE,
                              SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << "\n";
        exit(1);
    }   
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

void updateDisplay(const unsigned char* gfx){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int y = 0; y < VIDEO_HEIGHT; y++){
        for(int x = 0; x < VIDEO_WIDTH; x++){
            if(gfx[y * VIDEO_WIDTH + x]){
                SDL_Rect pixelRect = {
                    x * SCALE,
                    y * SCALE,
                    SCALE,
                    SCALE
                };

                SDL_RenderFillRect(renderer, &pixelRect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void destroyDisplay(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}