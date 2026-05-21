#ifndef Map_hpp
#define Map_hpp

#include "Game.hpp"


class Map
{
private:
    SDL_FRect SRC_RECT, DEST_RECT, SRC_BLACKHOLE, DEST_BLACKHOLE;
    int MAP[20][25];
public:
    Map();
    ~Map();
    void loadMap(int arr[20][25]);
    void drawMap();
};

#endif /* Map_hpp */
