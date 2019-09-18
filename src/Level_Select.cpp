#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

#include "general_SDL_funcs.h"
#include "Level_Select.h"

// TODO: define Level_Select funcs

Level_Select::Level_Select(SDL_Renderer* ren, std::vector<std::string> menu_options, int screen_height,
			int screen_width) : Menu(ren, "Level Select", menu_options, 10, 40, "res\\Courier font.ttf",
			true, screen_height, screen_width) {
	// add "Back" to the end of the menu options
	this->menu_options.push_back("Back");

	// TODO: define rows and columns 
	{
		SDL_Color color = {0, 0, 0, 255};
		SDL_Texture* tex;
		int w, h;
		try {
			tex = font_to_tex(ren, menu_font, menu_options[0], color);
			SDL_QueryTexture(tex, NULL, NULL, &w, &h);
			cleanup(tex);
		}
		catch (...) {
			cleanup(tex);
			throw "Level_Select::Level_Select() ERROR: Constructor could not be executed.\n";
		}
		// TODO: restrict poss_per_row further so it doesn't overlap with title or back buttons
		int poss_per_row = (screen_width - pad_x) / (pad_x + w);
		int poss_per_column = (screen_height - pad_y) / (pad_y + h);
		if (poss_per_row*poss_per_column < menu_options.size()) {
			throw "Level_Select::Level_Select() ERROR: Window is too small to display any levels.\n";
		}
		
		// TODO: somehow come up with the optimal number of rows and columns
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
