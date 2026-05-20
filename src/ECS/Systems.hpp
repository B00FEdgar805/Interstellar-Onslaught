//
//  Systems.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 5/19/26.
//

#ifndef Systems_hpp
#define Systems_hpp

#include "Registry.hpp"

#include <SDL3/SDL.h>

struct Systems
{
    void playerInputSystem(Registry& registry);
    
    void movementSystem(Registry& registry, float deltaTime);
    
    void renderSystem(Registry& registry, SDL_Renderer* renderer);
};
#endif /* Systems_hpp */
