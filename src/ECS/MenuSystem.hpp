#ifndef MenuSystem_hpp
#define MenuSystem_hpp

#include "Registry.hpp"
#include "Components/Button.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "PlayerSystems.hpp"

class Menu
{
private:
    Registry MAIN_MENU;
    Registry UPGRADE_MENU;
    Entity START_MAIN = MAIN_MENU.create();
    Entity OPTIONS_MAIN = MAIN_MENU.create();
    Entity QUIT_MAIN = MAIN_MENU.create();
    Entity UPGRADE_L = UPGRADE_MENU.create();
    Entity UPGRADE_M = UPGRADE_MENU.create();
    Entity UPGRADE_R = UPGRADE_MENU.create();
    Entity UPGRADE_BG = UPGRADE_MENU.create();

    SDL_FRect PAUSE_BACKGROUND;
    const Vector2D SIZE_MENU = {80, 32};
    const Vector2D SIZE_UPGRADE = {200, 200};

public:
    Menu();
    void initText(Registry& registry, PlayerSystems& player);
    void buttonSystem(SDL_Event& e);
    void renderSystemMain(SDL_Renderer* renderer);
    void renderSystemPause(SDL_Renderer* renderer);
    void renderUI(SDL_Renderer* renderer);
    void renderSystemUpgrade(SDL_Renderer* renderer);    
};
#endif /* MenuSystem_hpp */
