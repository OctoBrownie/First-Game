#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include <cmath> 

#include "general_SDL_funcs.h"
#include "Level_Select.h"

// TODO: define Level_Select funcs

Level_Select::Level_Select(SDL_Renderer* ren, std::vector<std::string> menu_options, int screen_height,
			int screen_width) : Menu(ren, "Level Select", menu_options, 20, 20, "res\\Courier font.ttf",
			true, screen_height, screen_width) {
	// add "Back" to the end of the menu options
	this->menu_options.push_back("Back");
	
	// had to reinit the option font and menu_font
	option_font_size = 30;
	menu_font = load_TTF_font(renderer, font_file, option_font_size);
	
	// init level array rectangle
	level_array_rect.x = 2*pad_x;
	level_array_rect.y = 3*pad_y + title_font_size;
	level_array_rect.w = screen_width - 4*pad_x;
	level_array_rect.h = screen_height - 6*pad_y - 2*title_font_size;
	
	// TODO: define number of rows and columns 
	{
		SDL_Texture* tex;
		int w, h;
		try {
			SDL_Color color = {0, 0, 0, 255};
			tex = font_to_tex(ren, menu_font, menu_options[0], color);
			SDL_QueryTexture(tex, NULL, NULL, &w, &h);
			cleanup(tex);
		}
		catch (...) {
			cleanup(tex);
			throw "Level_Select::Level_Select() ERROR: Constructor could not be executed.\n";
		}
		
		// come up with the optimal number of rows and columns
		int curr_rows = level_array_rect.w / (pad_x + w); 
		int curr_cols = ceil(menu_options.size() - 1 / curr_rows);
		int max_cols = level_array_rect.h / (pad_y + h);
		
		if (max_cols < curr_cols) {
			throw "Level_Select::Level_Select() ERROR: Window is too small to display all levels.\n";
		}
		int best_rows = curr_rows;
		int best_cols = curr_cols;
		while (max_cols >= curr_cols) {		// everything still fits
			// TODO: what makes this row-col pair the "best"?
			curr_rows--;
			curr_cols = ceil(menu_options.size() - 1 / curr_rows);
		}
	}
}

int Level_Select::render_menu() {
	Uint8 r, g, b, a;       // original draw color
    SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

    // menu background
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &menu_rect);

    SDL_Color title_color = {0, 0, 0, 255};

    // draw title
    SDL_Texture* title_tex = font_to_tex(ren, title_font, title, title_color);
    SDL_Rect title_dest_rect;
    SDL_QueryTexture(title_tex, NULL, NULL, &title_dest_rect.w, &title_dest_rect.h);

    title_dest_rect.y = menu_rect.y + pad_y;
	title_dest_rect.x = menu_rect.w / 2 - title_dest_rect.w / 2;
    SDL_RenderCopy(ren, title_tex, NULL, &title_dest_rect);
    cleanup(title_tex);

    // grid of levels, not including the "Back" option
    for (unsigned int i = 0; i < menu_options.size() - 1; i++) {
        // TODO: define the current row and column
        int curr_row =

        SDL_Color option_color = title_color;

        // TODO: Define text_box
        SDL_Rect text_box;
        // text_box.x = pad_x + curr_row

		Uint8 r, g, b, a;
		SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

        if (i == active_menu_option) {
            // level with highlighted background (b/c is active)
            SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
			option_color.r = 255;
			option_color.g = 255;
			option_color.b = 255;
        }
        else {
			// level with normal background
			SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
        }
		SDL_RenderFillRect(ren, &text_box);
		SDL_SetRenderDrawColor(ren, r, g, b, a);

		// the outline of the box
		SDL_RenderDrawRect(ren, &text_box);

		// actual text
		SDL_Texture* option_tex = font_to_tex(ren, menu_font, menu_options[i], option_color);

        // TODO: Change option_dest_rect
        SDL_Rect option_dest_rect;
        SDL_QueryTexture(option_tex, NULL, NULL, &option_dest_rect.w, &option_dest_rect.h);
        option_dest_rect.x = menu_rect.x + pad_x;
        option_dest_rect.y = menu_rect.y + title_font_size + 3*pad_y + (option_font_size + pad_y)*i;

        SDL_RenderCopy(ren, option_tex, NULL, &option_dest_rect);
    }
    // TODO: render the "Back" option
    // use title_color

    SDL_SetRenderDrawColor(ren, r, g, b, a);
    return 0;
}

void keyboard_input_down(const SDL_Event* event) {}

int get_option_at_x_y(const int x, const int y) {}
