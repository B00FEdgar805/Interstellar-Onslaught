//
//  GameObject.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/11/26.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include "Game.hpp"

class GameObject
{
private:
    int X_POS;
    int Y_POS;
    SDL_Texture* TEXTURE;
    SDL_FRect SRC_RECT, DEST_RECT;
    
public:
    GameObject(const char* texture_file, int x, int y);
    ~GameObject();
    void update();
    void render();
};

#endif /* GameObject_hpp */
