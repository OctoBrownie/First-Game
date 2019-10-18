#ifndef __Level_Select__
#define __Level_Select__

#include "Menu.h"

class Level_Select : public Menu {
protected:
	int rows, cols;
	SDL_Rect level_array_rect;

	void keyboard_input_down(const SDL_Event* event);
	int get_option_at_x_y(const int x, const int y);
public:
	Level_Select(SDL_Renderer* ren, std::vector<std::string> menu_options, int screen_height,
				int screen_width);
	// ~Level_Select();
	int render_menu();          // override to render formatted level array
};

#endif // __Level_Select__
