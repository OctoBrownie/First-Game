#ifndef __Puzzle__
#define __Puzzle__

#include <string>

class Puzzle {
private:
    // TODO: declare Puzzle funcs and properties
    int curr_x;
    int curr_y;

    int** map; // initialized as a ptr to a ptr b/c we don't know the size of array yet
    int map_size_x;
    int map_size_y;
public:
    // asdf
    std::string interpret_input(const SDL_Event* event);
    int render_menu();
};

#endif // __Puzzle__
