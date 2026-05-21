#ifndef TextureManager_hpp
#define TextureManager_hpp

#include "Game.hpp"

#include <string>
#include <unordered_map>

class TextureManager
{
private:
    static std::unordered_map<std::string, SDL_Texture*> TEXTURES;
public:
    static bool loadTexture(const std::string& id, const char* filepath);
    static SDL_Texture* getTexture(const std::string& id);
    static void unloadTexture(const std::string& id);
    static void clear();
    static void draw(const std::string& id,SDL_FRect source,SDL_FRect destination);
    static void drawRotated(const std::string& id, SDL_FRect source,SDL_FRect destination, float angle);
};


#endif /* TextureManager_hpp */
