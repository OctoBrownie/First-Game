#ifndef __Game_State__
#define __Game_State__

#include <SDL2/SDL.h>
#include <string>

#include "Puzzle.h"
#include "Menu.h"

class Game_State {
private:
    enum Game_Context {in_puzzle = 0, in_main, in_level_select, in_puzzle_menu};

    Game_Context curr_context;
    bool quit_now;
    SDL_Renderer* ren = nullptr;
    int screen_w, screen_h;
    std::string title;

	int main_context_input(const SDL_Event* event);
	int puzzle_context_input(const SDL_Event* event);
	int level_select_context_input(const SDL_Event* event);
	int puzzle_menu_context_input(const SDL_Event* event);

    int next_puzzle();

    int make_main_context();
    int make_puzzle_context();
    int make_level_select_context();
    int make_puzzle_menu_context();

public:
    Puzzle* curr_puzzle = nullptr;
    Menu* curr_menu = nullptr;

    Game_State(SDL_Renderer* renderer, const std::string& game_title, int screen_width, int screen_height);
    ~Game_State();
    int render_game();
    int check_input();

    const bool get_quit() {return quit_now;}
};

#endif // __Game_State__
