#ifndef Systems_hpp
#define Systems_hpp

#include "Registry.hpp"
#include "Camera.hpp"


#include <SDL3/SDL.h>

class Systems
{
private:
    float angle = 0.0f;
    //float speed = 220.0f;
    
public:
    void playerInputSystem();
        
    void movementSystem(float delta_time);
    
    void renderSystem(SDL_Renderer* renderer, const Camera2D& camera);
    
};
#endif /* Systems_hpp */
