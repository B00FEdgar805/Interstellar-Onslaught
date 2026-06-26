//
//  MenuSystem.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 6/22/26.
//

#ifndef MenuSystem_hpp
#define MenuSystem_hpp

#include "Registry.hpp"
#include "Components/Button.hpp"
#include <SDL3/SDL.h>

class Menu
{
private:
    
public:
    void buttonSystem(Registry& menu_registry, SDL_Event& e);
    
    void renderSystem(Registry& menu_registry, SDL_Renderer* renderer);
    
};
#endif /* MenuSystem_hpp */
