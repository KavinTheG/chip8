#include <SDL2/SDL.h>
#include <stdbool.h>

SDL_Window *window;
SDL_Renderer *renderer;

int scale = 8;

void init_graphics() {
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("chip8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64 * scale, 32 * scale, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
}


void draw_display(unsigned char *display[2048]) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int i = 0; i < 2048; i++) {
        if (display[i]) {
            int x = i % 64;
            int y = i / 64;
            SDL_Rect rect = {x * scale, y * scale, scale, scale};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);        
}   

void handle_keypress(unsigned char *keys, bool *running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            *running = false;
        } else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym)
            {

                case SDLK_1:
                    keys[1] = 1;
                    break;

                case SDLK_2:
                    keys[2] = 1;
                    break;  

                case SDLK_3:
                    keys[3] = 1;
                    break;
                
                case SDLK_4:
                    keys[0xc] = 1;
                    break;

                case SDLK_q:
                    keys[4] = 1;
                    break;  

                case SDLK_w:
                    keys[5] = 1;
                    break;
                
                case SDLK_e:
                    keys[6] = 1;
                    break;          

                case SDLK_r:
                    keys[7] = 1;
                    break;  

                case SDLK_a:
                    keys[8] = 1;
                    break;

                case SDLK_s:
                    keys[9] = 1;
                    break;

                case SDLK_d:
                    keys[0xa] = 1;
                    break;

                case SDLK_f:
                    keys[0xb] = 1;
                    break;

                case SDLK_z:
                    keys[0xe] = 1;
                    break;

                case SDLK_x:
                    keys[0] = 1;
                    break;

                case SDLK_c:
                    keys[0xd] = 1;
                    break;

                case SDLK_v:
                    keys[0xf] = 1;
                    break;

                default:
                    break;      
            
            }
        }
    }
}