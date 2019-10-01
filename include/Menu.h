#ifndef __Menu__
#define __Menu__

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

class Menu {
protected:
    std::vector<std::string> menu_options;
    int active_menu_option;					// set to -1 if nothing is active
    std::string title;

    int pad_x, pad_y;           // per button
    SDL_Rect menu_rect;         // for the entire menu
    bool fullscreen;
    bool is_active;
    int option_font_size, title_font_size;
    int longest_option_in_px;

    SDL_Renderer* ren = nullptr;
    TTF_Font* menu_font = nullptr;
    TTF_Font* title_font = nullptr;

    virtual void mouse_input_up(const SDL_Event* event);
    virtual void mouse_input_down_or_motion(const SDL_Event* event);	// down and motion do the same thing
    virtual void keyboard_input_down(const SDL_Event* event);
    virtual int keyboard_input_up(const SDL_Event* event);
    virtual int get_option_at_x_y(const int x, const int y);
public:
    Menu(SDL_Renderer* renderer, std::string title, std::vector<std::string> menu_options, int x_pad,
        int y_pad, std::string font_file, bool is_full_screen, int screen_height,
        int screen_width);

    // virtual allows the derived classes' versions of this function to be called (for destructor,
    // the Level_Select destructor will be called too)
    virtual ~Menu();
    virtual int render_menu();
    virtual std::string interpret_input(const SDL_Event* event);      // returns menu item clicked/keyed, else empty

    bool get_fullscreen() {return fullscreen;}
};

#endif // __Menu__
