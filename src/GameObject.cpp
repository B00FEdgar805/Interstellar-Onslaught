//
//  GameObject.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/11/26.
//

#include "GameObject.hpp"
#include "TextureManager.hpp"

GameObject::GameObject(const char* texture_file, SDL_Renderer* renderer, int x, int y)
{
    GAME_OBJECT_RENDERER = renderer;
    TEXTURE = TextureManager::loadTexture(texture_file, renderer);
    
    X_POS = x;
    Y_POS = y;
}

GameObject::~GameObject()
{
    
}

void GameObject::update()
{
   
    X_POS++;
    Y_POS++;
    SRC_RECT.h = 32;
    SRC_RECT.w = 32;
    SRC_RECT.x = 0;
    SRC_RECT.y = 0;
    
    DEST_RECT.x = X_POS;
    DEST_RECT.y = Y_POS;
    DEST_RECT.w = SRC_RECT.w;
    DEST_RECT.h = SRC_RECT.h;
}

void GameObject::render()
{
    if (!SDL_RenderTexture(GAME_OBJECT_RENDERER, TEXTURE, &SRC_RECT, &DEST_RECT))
    {
        SDL_Log("SDL_RenderTexture failed in GameObject render: %s\n", SDL_GetError());
    }
}
