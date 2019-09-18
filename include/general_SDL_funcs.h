#ifndef __general_SDL_funcs__
#define __general_SDL_funcs__

#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void log_SDL_error(std::ostream& os, const std::string& msg);
std::string get_res_path();
TTF_Font* load_TTF_font(SDL_Renderer* ren, const std::string& file, int font_size);
SDL_Texture* font_to_tex(SDL_Renderer* ren, TTF_Font* font, const std::string& message,
                           SDL_Color font_color);

void render_texture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y);

#endif // __general_SDL_funcs__
