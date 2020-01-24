#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>

#include "general_SDL_funcs.h"
#include "Menu.h"
#include "cleanup.h"

using namespace std;

Menu::Menu(SDL_Renderer* renderer, string title, vector<string> menu_options, int x_pad,
    int y_pad, string font_file, bool is_full_screen, int screen_height,
    int screen_width) : menu_options(menu_options), active_menu_option(-1), title(title),
    pad_x(x_pad), pad_y(y_pad), fullscreen(is_full_screen), ren(renderer) {

    if (is_full_screen) {
		is_active = true;
        option_font_size = 20;
        title_font_size = 40;
    }
    else {
    	is_active = false;
        option_font_size = 15;
        title_font_size = 24;
    }

    try {
		// load fonts
		menu_font = load_TTF_font(renderer, font_file, option_font_size);
		title_font = load_TTF_font(renderer, font_file, title_font_size);
    }
    catch (...) {
		cleanup(menu_font, title_font);
		throw "Menu::Menu() ERROR: Constructor could not be executed.\n";
    }

    // find the longest option in pixels
    {
    	// find the longest string
		int longest_str_i = 0;

		for (unsigned int i = 0; i < menu_options.size(); i++) {
			if (menu_options[i].size() > menu_options[longest_str_i].size())
				longest_str_i = i;
		}

		SDL_Texture* longest_str_tex;
		try {
			SDL_Color white = {255, 255, 255, 255};
			longest_str_tex = font_to_tex(renderer, menu_font, menu_options[longest_str_i], white);
			SDL_QueryTexture(longest_str_tex, NULL, NULL, &longest_option_in_px, NULL);
			cleanup(longest_str_tex);
		}
		catch (...) {
			cleanup(longest_str_tex);
			throw "Menu::Menu() ERROR: Constructor could not be executed.\n";
		}
		SDL_Color white = {255, 255, 255, 255};
		longest_str_tex = font_to_tex(renderer, menu_font, menu_options[longest_str_i], white);
		if (longest_str_tex == nullptr) {
			throw "Menu::Menu() ERROR: font_to_tex returned nullptr.\n";
		}
		SDL_QueryTexture(longest_str_tex, NULL, NULL, &longest_option_in_px, NULL);
		cleanup(longest_str_tex);
    }

    // fill in menu SDL_Rect
    if (is_full_screen) {
		// takes up the entire screen
        menu_rect.h = screen_height;
        menu_rect.w = screen_width;
        menu_rect.x = 0;
        menu_rect.y = 0;
    }
    else {
        // takes up part of the screen (centered)
        menu_rect.h = title_font_size + 2*pad_y + menu_options.size()*(option_font_size + y_pad) + pad_y;

		{
			SDL_Texture* title_texture;
			// width has to encompass entirety of all text (either bigger than options or title)
			try {
				// the title might be larger, so have to compare
				int title_width;
				SDL_Color white = {255, 255, 255, 255};
				title_texture = font_to_tex(renderer, title_font, title, white);
				if (title_texture == nullptr) {
					throw "Menu::Menu() ERROR: font_to_tex returned nullptr.\n";
				}
				SDL_QueryTexture(title_texture, NULL, NULL, &title_width, NULL);

				// so we know how wide to make the menu
				if (longest_option_in_px > title_width)
					menu_rect.w = 2*pad_x + longest_option_in_px;
				else
					menu_rect.w = 2*pad_x + title_width;
				cleanup(title_texture);
			}
			catch (...) {
				cleanup(title_texture);
				throw "Menu::Menu() ERROR: Constructor could not be executed.\n";
			}
		}
        menu_rect.x = screen_width / 2 - menu_rect.w / 2;
        menu_rect.y = screen_height / 2 - menu_rect.h / 2;
    }
}

int Menu::render_menu() {
    Uint8 r, g, b, a;       // original draw color
    SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

    // TODO: Implement is_active in menu rendering

    // menu background
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &menu_rect);

    SDL_Color menu_color = {0, 0, 0, 255};

    SDL_Texture* title_tex = font_to_tex(ren, title_font, title, menu_color);
	if (title_tex == nullptr) {
		return 1;
	}
    SDL_Rect title_dest_rect;
    SDL_QueryTexture(title_tex, NULL, NULL, &title_dest_rect.w, &title_dest_rect.h);
    title_dest_rect.y = menu_rect.y + pad_y;

    if (fullscreen) {
        // centered title
        title_dest_rect.x = menu_rect.w / 2 - title_dest_rect.w / 2;
    }
    else {
        // left aligned title
        title_dest_rect.x = menu_rect.x + pad_x;
    }
    SDL_RenderCopy(ren, title_tex, NULL, &title_dest_rect);
    cleanup(title_tex);

    // left aligned options
    for (unsigned int i = 0; i < menu_options.size(); i++) {
        SDL_Texture* option_tex = font_to_tex(ren, menu_font, menu_options[i], menu_color);
		if (option_tex == nullptr) {
			return 1;
		}

        SDL_Rect option_dest_rect;
        SDL_QueryTexture(option_tex, NULL, NULL, &option_dest_rect.w, &option_dest_rect.h);
        option_dest_rect.x = menu_rect.x + pad_x;
        option_dest_rect.y = menu_rect.y + title_font_size + 3*pad_y + (option_font_size + pad_y)*i;

        if (active_menu_option != -1 && i == active_menu_option) {
            Uint8 r, g, b, a;
            SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

            SDL_Rect highlight_rect;
            highlight_rect.x = option_dest_rect.x;
            highlight_rect.y = option_dest_rect.y - pad_y;
            highlight_rect.h = option_dest_rect.h + 2*pad_y;

            // box width defined by the largest of the options
            highlight_rect.w = longest_option_in_px + pad_x;

            SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
            SDL_RenderFillRect(ren, &highlight_rect);

            SDL_SetRenderDrawColor(ren, r, g, b, a);
        }

        SDL_RenderCopy(ren, option_tex, NULL, &option_dest_rect);
    }

    SDL_SetRenderDrawColor(ren, r, g, b, a);
    return 0;
}

string Menu::interpret_input(const SDL_Event* event) {
    // only returns a non-empty string if something was actually SELECTED
    // as in something was clicked or the enter/space key was pressed while selecting something

    switch(event->type) {

    // handle keys/mouse DOWN (when selection is occurring)
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEMOTION:
        this->mouse_input_down_or_motion(event);
        return "";			// something WILL be active, but it hasn't been clicked
    case SDL_KEYDOWN:
        this->keyboard_input_down(event);
        return "";

    // handle keys/mouse UP (when the selection has been made)
    case SDL_MOUSEBUTTONUP:
    	{
			// TODO: check if is_active is CHANGED (to false or true)
			// TODO: Implement is_active switching in mouse and key up
			// mouse_input_up(event);

    		// continue and return active_menu_option if is_active is still true
			// active_menu_option = -1 if nothing active
			if (active_menu_option == -1)
				return "";

			int res = active_menu_option;
			active_menu_option = -1;
			return menu_options[res];
        }
    case SDL_KEYUP:
    	{
			// will return -1 if nothing selected, otherwise returns index
			int res = keyboard_input_up(event);
			if (res == -1)
				return "";

			active_menu_option = -1;
			return menu_options[res];
    	}
    }
}

void Menu::mouse_input_up(const SDL_Event* event) {
	// use the coordinates of the mouse to check if menu still active
	// DOESN'T change the active option
	if (event->button.button == SDL_BUTTON_LEFT && event->button.x >= menu_rect.x &&
		event->button.x <= menu_rect.x + menu_rect.w && event->button.y >= menu_rect.y &&
		event->button.y <= menu_rect.y + menu_rect.h) {
		// is within the menu
		is_active = true;
	}
	// lies outside the menu
	is_active = false;
}

void Menu::mouse_input_down_or_motion(const SDL_Event* event) {
	// use the new coordinates to change the active item
	// decide which item is active

	// have to check for if left button is clicked
	int x, y;

	if (event->type == SDL_MOUSEBUTTONDOWN) {
		x = event->button.x;
		y = event->button.y;
		if (event->button.button != SDL_BUTTON_LEFT) { return; }
	}
	else {		// SDL_MOUSEMOTION
		x = event->motion.x;
		y = event->motion.y;
		if (event->motion.state != SDL_BUTTON_LMASK) { return; }
	}

	int option = get_option_at_x_y(x, y);
	if (option != -1)
		active_menu_option = option;
	else
		active_menu_option = -1;
}

void Menu::keyboard_input_down(const SDL_Event* event) {
	// to check for repeats
	static int repeats = 0;

	if (event->key.keysym.scancode == SDL_SCANCODE_W || event->key.keysym.scancode == SDL_SCANCODE_S) {
		if (event->key.repeat == 0) {
			repeats = 0;
		}
		else {
			float secs = 0.5;
			// TODO: Possibly change the "5" to an average fps (probably divided by something)
			int frames = secs*5;
			repeats = (repeats + 1) % frames;
		}

		// cycles through the menu options
		if (repeats == 0) {
			if (active_menu_option == -1)
				active_menu_option = 0;
			else if (event->key.keysym.scancode == SDL_SCANCODE_W) {
				if (active_menu_option == 0)
					active_menu_option = menu_options.size() - 1;
				else
					active_menu_option = (active_menu_option - 1) % menu_options.size();
			}
			else if (event->key.keysym.scancode == SDL_SCANCODE_S)
				active_menu_option = (active_menu_option + 1) % menu_options.size();
		}
	}
}

int Menu::keyboard_input_up(const SDL_Event* event) {
	// returns the index of the option clicked

	// if it's a key that can initiate clicking
	if (event->key.keysym.scancode == SDL_SCANCODE_SPACE || event->key.keysym.scancode == SDL_SCANCODE_RETURN
	|| event->key.keysym.scancode == SDL_SCANCODE_KP_ENTER || event->key.keysym.scancode == SDL_SCANCODE_E) {
		return active_menu_option;
	}

	return -1;
}

int Menu::get_option_at_x_y(const int x, const int y) {
	// returns the index of the option at some (x,y) relative to the window
	// returns -1 if not on an option

	// x ranges from (menu_rect.x + pad_x) to (menu_rect.x + pad_x + longest_option_in_px)
	// y ranges from (menu_rect.y + title_font_size + 3*pad_y + (option_font_size + pad_y)*num) to
	// (menu_rect.y + title_font_size + 3*pad_y + (option_font_size + pad_y)*(num + 1))

	if (menu_rect.x + pad_x <= x && x <= menu_rect.x + pad_x + longest_option_in_px) {
		// is in the possible range of x values, so must check for y values
		unsigned int i = (y - menu_rect.y - title_font_size - 3*pad_y)/(option_font_size + pad_y);
		if (i < menu_options.size()) 	// i is a valid index
			return i;
	}
	return -1;		// outside possible x values
}
