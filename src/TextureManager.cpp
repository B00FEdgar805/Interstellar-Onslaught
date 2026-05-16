//
//  TextureManager.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/10/26.
//

#include "TextureManager.hpp"

SDL_Texture* TextureManager::loadTexture(const char* filename)
{
    SDL_Surface* temp = IMG_Load(filename);
    if(temp == NULL)
    {
        SDL_Log("Error image not loaded ", filename);
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::RENDERER, temp);
    SDL_DestroySurface(temp);
    return texture;
}

void TextureManager::draw(SDL_Texture* texture, SDL_FRect source, SDL_FRect destination)
{
    SDL_RenderTexture(Game::RENDERER, texture, &source, &destination);
}

void TextureManager::drawRotated(SDL_Texture* texture, SDL_FRect source, SDL_FRect destination, float angle)
{
    SDL_RenderTextureRotated(Game::RENDERER, texture, &source, &destination, angle, nullptr, SDL_FLIP_NONE);
}
