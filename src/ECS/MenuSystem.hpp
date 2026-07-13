#ifndef MenuSystem_hpp
#define MenuSystem_hpp

#include "Registry.hpp"
#include "Components/Button.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class Menu
{
private:
    Registry MAIN_MENU;
    Registry PAUSE_MENU;
    Entity START_MAIN = MAIN_MENU.create();
    Entity OPTIONS_MAIN = MAIN_MENU.create();
    Entity QUIT_MAIN = MAIN_MENU.create();
    SDL_FRect PAUSE_BACKGROUND;
    const Vector2D SIZE = {80,32};

public:
    Menu();
    void initText();
    void buttonSystem(SDL_Event& e);
    void renderSystemMain(SDL_Renderer* renderer);
    void renderSystemPause(SDL_Renderer* renderer);
    
};
#endif /* MenuSystem_hpp */
