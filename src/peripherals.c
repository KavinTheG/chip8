#include <SDL2/SDL.h>
#include <stdbool.h>

SDL_Window *window;
SDL_Renderer *renderer;

int scale = 8;

const static short keyboard[0x10] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F
};

void init_graphics() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * scale, 32 * scale, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
}


void draw_display(unsigned char *display[2048]) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 32; i++) {
        for (int j = 0; j < 64; j++) {
            if (display[i * 64 + j] == 1) {
                SDL_Rect rect = {j * scale, i * scale, scale, scale};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(renderer);        
}   

void quit_graphics() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit(); 
}

void handle_keypress(unsigned char *keys, bool *running) {
    SDL_Event event;

    for (int i = 0; i < 16; i++) {
    //    keys[i] = 0;
    }

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        } else if (event.type == SDL_KEYDOWN) {
            for (int i = 0; i < 16; i++) {
                if (event.key.keysym.sym == keyboard[i]) {
                    keys[i] = 1;
                }
            }
        } else if (event.type == SDL_KEYUP) {
            for (int i = 0; i < 16; i++) {
                if (event.key.keysym.sym == keyboard[i]) {
                    keys[i] = 0;
                }
            }
        }
    }
}

