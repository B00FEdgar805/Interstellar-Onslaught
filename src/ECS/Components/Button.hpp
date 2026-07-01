#ifndef Button_hpp
#define Button_hpp

#include "Component.hpp"
#include "../../GLOBALS.hpp"
#include "../../TextureManager.hpp"

class Button final : public BaseComponent
{
private:
    std::string TEXTURE_ID;
    SDL_FRect SOURCE, DESTINATION;
    GLOBALS::GameState BUTTON_STATE;

public:
    Button() = default;
    Button(const std::string &textureID, const Vector2D& destination , const Vector2D& size)
    {
        TEXTURE_ID = textureID;
        SOURCE.x = 0;
        SOURCE.y = 0;
        SOURCE.w = size.x;
        SOURCE.h = size.y;
        DESTINATION.w = size.x;
        DESTINATION.h = size.y;
        DESTINATION.x = destination.x;
        DESTINATION.y = destination.y;
    }
    
    std::function<void()> onClick;
    
    void setState(GLOBALS::GameState state)
    {
        BUTTON_STATE = state;
    }
    
    GLOBALS::GameState getState()
    {
        return BUTTON_STATE;
    }
    
    void setButton(int row)
    {
        SOURCE.y = SOURCE.h * (row - 1);
    }
    
    void hovering(bool hover)
    {
        SOURCE.x = SOURCE.w * hover;
    }
    
    int x()
    {
        return DESTINATION.x;
    }
    
    int y()
    {
        return DESTINATION.y;
    }
    
    int w()
    {
        return DESTINATION.w;
    }
    
    int h()
    {
        return DESTINATION.h;
    }
    
    void draw()
    {
        TextureManager::draw(TEXTURE_ID, SOURCE, DESTINATION);
    }
};


#endif /* Button_hpp */
