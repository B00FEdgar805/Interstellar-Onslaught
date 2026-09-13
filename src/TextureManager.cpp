#include "TextureManager.hpp"

std::unordered_map<std::string, SDL_Texture*> TextureManager::TEXTURES; // Stores all loaded textures

bool TextureManager::loadTexture(const std::string& id, const char* filepath)   // Creates and ID and stores texture for later use
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

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);
    unloadTexture(id);

    TEXTURES[id] = texture;

    return true;
}

SDL_Texture* TextureManager::getTexture(const std::string& id)
{
    auto it = TEXTURES.find(id);

    if (it == TEXTURES.end())
    {
        return nullptr;
    }

    return it -> second;
}

void TextureManager::unloadTexture(const std::string& id)
{
    auto it = TEXTURES.find(id);

    if (it == TEXTURES.end())
    {
        return;
    }

    if (it->second != nullptr)
    {
        SDL_DestroyTexture(it->second);
    }

    TEXTURES.erase(it);
}

void TextureManager::clear()    // Unloads all textures
{
    for (auto& [id, texture] : TEXTURES)
    {
        (void)id;

        if (texture != nullptr)
        {
            SDL_DestroyTexture(texture);
        }
    }

    TEXTURES.clear();
}

void TextureManager::drawRect(SDL_FRect source, SDL_FRect destination)
{
    
}

void TextureManager::draw(const std::string& id, SDL_FRect source, SDL_FRect destination)
{
    SDL_Texture* texture = getTexture(id);

    if (texture == nullptr)
    {
        SDL_Log("Texture not found id:%s", id.c_str());
        return;
    }

    SDL_RenderTexture(Game::RENDERER, texture, &source, &destination);
}

void TextureManager::drawRotated(const std::string& id, SDL_FRect source, SDL_FRect destination, float angle)
{
    SDL_Texture* texture = getTexture(id);

    if (texture == nullptr)
    {
        return;
    }

    SDL_RenderTextureRotated(Game::RENDERER, texture, &source, &destination, angle, nullptr, SDL_FLIP_NONE);
    
}
