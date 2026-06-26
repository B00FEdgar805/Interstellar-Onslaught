//
//  MenuSystem.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 6/22/26.
//

#include "MenuSystem.hpp"
#include "../Game.hpp"

void Menu::buttonSystem(Registry &menu_registry, SDL_Event &e)
{
    auto view = menu_registry.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        
        float x = 0.0f;
        float y = 0.0f;
        
        SDL_GetMouseState(&x, &y);
        
        if (x >= button.x() && x <= (button.x() + button.w()) && y >= button.y() && y <= (button.y() + button.h()))
        {
            button.hovering();
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
            {
                Game::CURRENT_STATE = Game::STATE_GAMEPLAY;
            }
        }
        
    }
}

void Menu::renderSystem(Registry &menu_registry, SDL_Renderer *renderer)
{
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    if (!SDL_RenderClear(renderer))
    {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    }
    
    auto view = menu_registry.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
    SDL_RenderPresent(renderer);

}
