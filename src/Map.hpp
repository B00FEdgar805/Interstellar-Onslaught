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
    SDL_FRect SRC_RECT, DEST_RECT, SRC_BLACKHOLE, DEST_BLACKHOLE;
    SDL_Texture* STARS;
    SDL_Texture* STAR;
    SDL_Texture* PLANET;
    SDL_Texture* BLACKHOLE;
    SDL_Texture* SPACE;

    int MAP[20][25];
public:
    Map();
    ~Map();
    void loadMap(int arr[20][25]);
    void drawMap();
};

#endif /* Map_hpp */
