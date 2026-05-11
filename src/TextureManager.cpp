//
//  TextureManager.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/10/26.
//

#include "TextureManager.hpp"

SDL_Texture* TextureManager::loadTexture(const char* filename, SDL_Renderer* renderer)
{
    SDL_Surface* temp = IMG_Load(filename);
    if(temp == NULL)
    {
        SDL_Log("Error image not loaded ", filename);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_DestroySurface(temp);
    return texture;
}
