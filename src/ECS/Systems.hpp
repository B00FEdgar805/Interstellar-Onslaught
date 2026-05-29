#ifndef Systems_hpp
#define Systems_hpp

#include "Registry.hpp"
#include "Camera.hpp"


#include <SDL3/SDL.h>

struct Systems
{
    void playerInputSystem(Registry& registry);
        
    void movementSystem(Registry& registry, float deltaTime);
    
    void renderSystem(Registry& registry, SDL_Renderer* renderer, const Camera2D& camera);
    
};
#endif /* Systems_hpp */
