//
//  TextureManager.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/10/26.
//

#ifndef TextureManager_hpp
#define TextureManager_hpp

#include "Game.hpp"

class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filename, SDL_Renderer* renderer);
};

#endif /* TextureManager_hpp */
