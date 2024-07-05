#include <SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;

int scale = 8;



void init_graphics() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * scale, 32 * scale, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
}


void draw_display(unsigned char *display) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 2048; i++) {
        if (display[i] == 1) {
            int x = i % 64;
            int y = i / 64;
            SDL_Rect rect = {x * scale, y * scale, scale, scale};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);        
}   

void handle_keypress() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym)
            {
            
            }
        }
    }
}