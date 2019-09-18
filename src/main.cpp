#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <cstdlib>
#include <string>

#include "cleanup.h"
#include "general_SDL_funcs.h"
#include "Game_State.h"

using namespace std;

Game_State* game = nullptr;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string TITLE = "My First Game";


int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        log_SDL_error(cout, "SDL_Init");
        return 1;
    }

    if (TTF_Init() != 0) {
        log_SDL_error(cout, "TTF_Init");
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(TITLE.c_str(), 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        log_SDL_error(cout, "SDL_CreateWindow");
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr) {
        log_SDL_error(cout, "SDL_CreateRenderer");
        cleanup(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    game = new Game_State(ren, TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (game == nullptr) {
        cout << "Game_State error: Couldn't create new game context.";
        cleanup(window, ren);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    while (game->get_quit() == false) {
        game->check_input();

        // rendering
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderClear(ren);

        game->render_game();

        SDL_RenderPresent(ren);
        SDL_Delay(100);
    }

    delete game;
    cleanup(window, ren);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
