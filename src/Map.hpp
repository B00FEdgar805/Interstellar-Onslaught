//
//  Map.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/11/26.
//

#ifndef Map_hpp
#define Map_hpp

#include "Game.hpp"

class Map
{
private:
    SDL_FRect SRC_RECT, DEST_RECT;
    SDL_Texture* BACKGROUND;
    int MAP[20][25];
public:
    Map();
    ~Map();
    void loadMap(int arr[20][25]);
    void drawMap();
};

#endif /* Map_hpp */
