#ifndef Sprite_hpp
#define Sprite_hpp

#include "Component.hpp"
#include "../../TextureManager.hpp"

class Sprite final : public BaseComponent
{
private:
    std::string TEXTURE_ID;
    SDL_FRect SOURCE, DESTINATION;
public:
    Sprite() = default;
   
    Sprite(const std::string &textureID)    // Initializes the sprite to be 32 by 32 and use the whole image and get texture id for lookup later
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
    
    void setPosition(const Vector2D& v)
    {
        DESTINATION.x = v.x;
        DESTINATION.y = v.y;
    }
    
    void draw() // Called by Systems to draw sprite
    {
        TextureManager::draw(TEXTURE_ID, SOURCE, DESTINATION);
       // SDL_Log("Sprite drawing");
    }
};

#endif /* Sprite_hpp */
