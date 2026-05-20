//
//  TextureManager.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/10/26.
//

//#include "TextureManager.hpp"
/*
SDL_Texture* TextureManager::loadTexture(const char* filepath)
{
    SDL_Surface* temp = IMG_Load(filepath);
    if(temp == NULL)
    {
        SDL_Log("Error image not loaded ");
        SDL_Log("%s" ,filepath);
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
*/

#include "TextureManager.hpp"

std::unordered_map<std::string, SDL_Texture*> TextureManager::textures;

bool TextureManager::loadTexture(const std::string& id, const char* filepath)
{
    SDL_Surface* temp = IMG_Load(filepath);

    if (temp == nullptr)
    {
        SDL_Log("Error image not loaded: %s", filepath);
        SDL_Log("SDL error: %s", SDL_GetError());
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::RENDERER, temp);

    SDL_DestroySurface(temp);

    if (texture == nullptr)
    {
        SDL_Log("Error texture not created: %s", filepath);
        SDL_Log("SDL error: %s", SDL_GetError());
        return false;
    }

    unloadTexture(id);

    textures[id] = texture;

    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& id)
{
    auto it = textures.find(id);

    if (it == textures.end())
    {
        return nullptr;
    }

    return it->second;
}

void TextureManager::unloadTexture(const std::string& id)
{
    auto it = textures.find(id);

    if (it == textures.end())
    {
        return;
    }

    if (it->second != nullptr)
    {
        SDL_DestroyTexture(it->second);
    }

    textures.erase(it);
}

void TextureManager::clear()
{
    for (auto& [id, texture] : textures)
    {
        (void)id;

        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
        }
    }

    textures.clear();
}

void TextureManager::draw(
    const std::string& id,
    SDL_FRect source,
    SDL_FRect destination
)
{
    SDL_Texture* texture = getTexture(id);

    if (texture == nullptr)
    {
        SDL_Log("Texture not found");
        return;
    }

    SDL_RenderTexture(
        Game::RENDERER,
        texture,
        &source,
        &destination
    );
}

void TextureManager::drawRotated(
    const std::string& id,
    SDL_FRect source,
    SDL_FRect destination,
    float angle
)
{
    SDL_Texture* texture = getTexture(id);

    if (texture == nullptr)
    {
        return;
    }

    SDL_RenderTextureRotated(
        Game::RENDERER,
        texture,
        &source,
        &destination,
        angle,
        nullptr,
        SDL_FLIP_NONE
    );
    
}
