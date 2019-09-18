#ifndef __cleanup_h__
#define __cleanup_h__

#include <utility>
#include <SDL2/SDL.h>

// generic cleanup function, called when multiple args given (later expands to specializations)
template<typename T, typename... Args>  // "..." can expand to as many args as it wants
void cleanup(T* t, Args&&... args) {
    cleanup(t);                                 // clean one at a time
    cleanup(std::forward<Args>(args)...);       // recurses to make sure we get everything
}

// 'specializations' of cleanup, when it is passed something specific
template<>
inline void cleanup<SDL_Window>(SDL_Window* win) {                     // SDL_Window
    if (!win) {         // window is a nullptr already
        return;
    }
    SDL_DestroyWindow(win);
}

template<>
inline void cleanup<SDL_Renderer>(SDL_Renderer* ren) {
    if (!ren) {
        return;
    }
    SDL_DestroyRenderer(ren);
}

template<>
inline void cleanup<SDL_Texture>(SDL_Texture* tex) {
    if (!tex) {
        return;
    }
    SDL_DestroyTexture(tex);
}

template<>
inline void cleanup<SDL_Surface>(SDL_Surface* surf) {
    if (!surf) {
        return;
    }
    SDL_FreeSurface(surf);
}

template<>
inline void cleanup<TTF_Font>(TTF_Font* font) {
    if (!font) {
        return;
    }
    TTF_CloseFont(font);
}

#endif // __cleanup_h__
