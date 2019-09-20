#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include <vector>

#include "Game_State.h"
#include "Menu.h"
#include "Puzzle.h"
#include "Level_Select.h"

using namespace std;

Game_State::Game_State(SDL_Renderer* renderer, const std::string& game_title, int screen_width,
        int screen_height) : curr_context(in_main), quit_now(false), ren(renderer),
        screen_w(screen_width), screen_h(screen_height), title(game_title) {

    if(make_main_context() == 1) {
		quit_now = true;
    }
}

Game_State::~Game_State() {
    if (curr_puzzle != nullptr)
        delete curr_puzzle;
    if (curr_menu != nullptr)
        delete curr_menu;
    // renderer shouldn't be deleted b/c it was passed in via constructor
}

int Game_State::check_input() {
    SDL_Event curr_event;

    while (SDL_PollEvent(&curr_event)) {
        if (curr_event.type == SDL_QUIT) {
            // no need to handle other events once quit is called
            quit_now = true;
            break;
        }

        switch(curr_context) {
		case in_main:
			main_context_input(&curr_event);
			break;
		case in_level_select:
			level_select_context_input(&curr_event);
			break;
		case in_puzzle:
			puzzle_context_input(&curr_event);
			break;
		case in_puzzle_menu:
			puzzle_menu_context_input(&curr_event);
			break;
        }
    }
    return 0;
}


int Game_State::main_context_input(const SDL_Event* event) {
	switch(event->type) {
	case SDL_KEYDOWN:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEMOTION:
		curr_menu->interpret_input(event);
		break;

	case SDL_KEYUP:
	case SDL_MOUSEBUTTONUP:
		{
			string result = curr_menu->interpret_input(event);

			if (result == "Play") {
				cout << "Play has been selected.\n";
				make_puzzle_context();
			}
			else if (result == "Level Select") {
				cout << "Level Select selected.\n";
				make_level_select_context();
			}
			else if (result == "Quit") {
				quit_now = true;
			}
		}
		break;
	}
}

int Game_State::puzzle_context_input(const SDL_Event* event) {
	// TODO: possible optimization
	switch(event->type) {
	case SDL_KEYDOWN:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEMOTION:
		{
			string result = curr_puzzle->interpret_input(event);
			// based on returns, MAYBE do something since mouse button could potentially get to puzzle end
		}
		break;

	case SDL_KEYUP:
	case SDL_MOUSEBUTTONUP:
		{
			string result = curr_puzzle->interpret_input(event);
			// change puzzle or go to puzzle menu
		}
		break;
	}
}

int Game_State::level_select_context_input(const SDL_Event* event) {
	switch(event->type) {
	case SDL_KEYDOWN:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEMOTION:
		curr_menu->interpret_input(event);
		break;

	case SDL_KEYUP:
	case SDL_MOUSEBUTTONUP:
		{
			string result = curr_menu->interpret_input(event);

			// send to a puzzle or back to main
		}
		break;
	}
}

int Game_State::puzzle_menu_context_input(const SDL_Event* event) {
	switch(event->type) {
	case SDL_KEYDOWN:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEMOTION:
		curr_menu->interpret_input(event);
		break;

	case SDL_KEYUP:
	case SDL_MOUSEBUTTONUP:
		{
			string result = curr_menu->interpret_input(event);

			// based on returns, either pass back to puzzle or go to main
		}
		break;
	}
}

int Game_State::make_main_context() {
	vector<string> main_menu_options = {"Play", "Level Select", "Quit"};
    try {
		Menu* temp_menu = new Menu(ren, title, main_menu_options, 20, 10,
								   "res\\Courier font.ttf", true, screen_h, screen_w);
    }
    catch (const char* msg) {
        cout << "Menu creation error: Menu constructor couldn't be called.\n";
		cout << msg;
        quit_now = true;
        return 1;
    }

	delete curr_puzzle;
	curr_puzzle = nullptr;

	delete curr_menu;
    curr_menu = temp_menu;

    curr_context = in_main;
    return 0;
}

int Game_State::make_puzzle_context() {
	// TODO: Game_State::make_puzzle_context
	curr_context = in_puzzle;
	return 0;
}

int Game_State::make_level_select_context() {
	// TODO: Game_State::make_level_select_context
	curr_context = in_level_select;
	return 0;
}

int Game_State::make_puzzle_menu_context() {
	// TODO: Game_State::make_puzzle_menu_context
	curr_context = in_puzzle_menu;
	return 0;
}

int Game_State::next_puzzle() {
    // TODO: Game_State::next_puzzle()
    return 0;
}

int Game_State::render_game() {
    // TODO: Game_State::render_game()
    if (curr_context == in_main) {
		curr_menu->render_menu();
    }
    return 0;
}
