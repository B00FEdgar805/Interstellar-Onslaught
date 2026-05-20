//
//  TextureManager.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/10/26.
//

#ifndef TextureManager_hpp
#define TextureManager_hpp

#include "Game.hpp"

/*
class TextureManager
{
public:
    static SDL_Texture* loadTexture(const char* filepath);
    static void draw(SDL_Texture* texture, SDL_FRect source, SDL_FRect destination);
    static void drawRotated(SDL_Texture* texture, SDL_FRect source, SDL_FRect destination, float angle);

};


*/

#include <string>
#include <unordered_map>

class TextureManager {
public:
    static bool loadTexture(
        const std::string& id,
        const char* filepath
    );

    static SDL_Texture* getTexture(const std::string& id);

    static void unloadTexture(const std::string& id);

    static void clear();

    static void draw(
        const std::string& id,
        SDL_FRect source,
        SDL_FRect destination
    );

    static void drawRotated(
        const std::string& id,
        SDL_FRect source,
        SDL_FRect destination,
        float angle
    );

private:
    static std::unordered_map<std::string, SDL_Texture*> textures;
};


#endif /* TextureManager_hpp */
