#ifndef __Puzzle__
#define __Puzzle__

#include <string>

class Puzzle {
private:
    // TODO: declare Puzzle funcs and properties
public:
    // asdf
    std::string interpret_input(const SDL_Event* event);
    int render_menu();
};

#endif // __Puzzle__
