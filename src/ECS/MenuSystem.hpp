#ifndef MenuSystem_hpp
#define MenuSystem_hpp

#include "Registry.hpp"
#include "Components/Button.hpp"
#include <SDL3/SDL.h>

class Menu
{
private:
    Registry UI;
    Entity START = UI.create();
    Entity OPTIONS = UI.create();
    Entity QUIT = UI.create();



public:
    Menu();
    void buttonSystem(SDL_Event& e);
    void renderSystem(SDL_Renderer* renderer);
    
};
#endif /* MenuSystem_hpp */
