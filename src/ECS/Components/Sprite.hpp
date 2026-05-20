//
//  Sprite.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/19/26.
//

#ifndef Sprite_hpp
#define Sprite_hpp

#include "Component.hpp"
#include "../../TextureManager.hpp"

class Sprite final : public BaseComponent
{
private:
    //SDL_Texture* TEXTURE;
    std::string TEXTURE_ID;
    SDL_FRect SOURCE, DESTINATION;
public:
    Sprite() = default;
    /*
    Sprite(const char* filepath)
    {
        //TEXTURE = TextureManager::loadTexture(filepath);
        SOURCE.x = 0;
        SOURCE.y = 0;
        SOURCE.w = 32;
        SOURCE.h = 32;
        DESTINATION.w = 32;
        DESTINATION.h = 32;
        DESTINATION.x = 0;
        DESTINATION.y = 0;
    }
    */
    
    Sprite(const std::string &textureID)
    {
        TEXTURE_ID = textureID;
        SOURCE.x = 0;
        SOURCE.y = 0;
        SOURCE.w = 32;
        SOURCE.h = 32;
        DESTINATION.w = 32;
        DESTINATION.h = 32;
        DESTINATION.x = 0;
        DESTINATION.y = 0;
    }
    
    virtual ~Sprite(){}
    
    void setSize(int w, int h)
    {
        DESTINATION.w = w;
        DESTINATION.h = h;
    }
    void x(int x)
    {
        DESTINATION.x = x;
    }
    
    void y(int y)
    {
        DESTINATION.y = y;
    }
    
    void draw()
    {
        TextureManager::draw(TEXTURE_ID, SOURCE, DESTINATION);
       // SDL_Log("Sprite drawing");
    }
};

#endif /* Sprite_hpp */
