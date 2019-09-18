#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "general_SDL_funcs.h"

void log_SDL_error(std::ostream& os, const std::string& msg) {
    os << msg << " error: " << SDL_GetError() << std::endl;
}

std::string get_res_path() {
    #ifdef _WIN32
        const char PATH_SEP = '\\';
    #else
        const char PATH_SEP = '/';
    #endif // _WIN32

    static std::string res_path;

    if (res_path.empty()) {
        char* base_path = SDL_GetBasePath();
        if (base_path) {
            res_path = base_path;
            SDL_free(base_path);
        }
        else {
            log_SDL_error(std::cout, "SDL_GetBasePath");
            return "";
        }
        res_path = res_path + "res" + PATH_SEP;
    }
    return res_path;
}

TTF_Font* load_TTF_font(SDL_Renderer* ren, const std::string& file, int font_size) {
	TTF_Font* my_font = TTF_OpenFont(file.c_str(), font_size);
	if (my_font == nullptr) {
        log_SDL_error(std::cout, "TTF_OpenFont");
        return nullptr;
	}
    return my_font;
}

SDL_Texture* font_to_tex(SDL_Renderer* ren, TTF_Font* font, const std::string& message,
                           SDL_Color font_color) {
	SDL_Surface* my_surface = TTF_RenderText_Solid(font, message.c_str(), font_color);
    if (my_surface == nullptr) {
        log_SDL_error(std::cout, "TTF_RenderText_Solid");
        return nullptr;
    }

    SDL_Texture* my_tex = SDL_CreateTextureFromSurface(ren, my_surface);
    if (my_tex == nullptr) {
        log_SDL_error(std::cout, "SDL_CreateTextureFromSurface");
    }
    return my_tex;
}

void render_texture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y) {
    SDL_Rect bounds;

    // setting x and y bounds
    bounds.x = x;
    bounds.y = y;
    // sets the height and width bounds
    SDL_QueryTexture(tex, NULL, NULL, &bounds.w, &bounds.h);

    // render with bounds
    SDL_RenderCopy(ren, tex, NULL, &bounds);
}
