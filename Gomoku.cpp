#include <iostream>
#include "game.hpp"

int main(){
    SDL_Window *window;
    SDL_Renderer *renderer;
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        SDL_GetError();
        return 0;
    }
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    window = SDL_CreateWindow(
        "Gomoku",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        800,
        800,
        SDL_WINDOW_SHOWN
    );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Loadtexture(renderer);
    // LoadCard(renderer);
    // GamePresent(renderer);

    // Game(renderer);
    SDL_RenderPresent(renderer);

    // destroyallsource();
	SDL_DestroyRenderer(renderer); 
	SDL_DestroyWindow(window); 
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    return 0;
}
