#include "MenuSystem.hpp"
#include "../Globals.hpp"

Menu::Menu()
{
    MAIN_MENU.add(START_MAIN, Button("Buttons", Vector2D(300, 100), Vector2D(200, 120)));
    Button* start_main = MAIN_MENU.get<Button>(START_MAIN);
    start_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    MAIN_MENU.add(OPTIONS_MAIN, Button("Buttons", Vector2D(300, 300), Vector2D(200, 120)));
    Button* options_main = MAIN_MENU.get<Button>(OPTIONS_MAIN);
    options_main -> setButton(2);
    
    MAIN_MENU.add(QUIT_MAIN, Button("Buttons", Vector2D(300, 500), Vector2D(200, 120)));
    Button* quit_main = MAIN_MENU.get<Button>(QUIT_MAIN);
    quit_main -> setButton(3);
    quit_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_EXIT;
    };
    //START_BUTTON = UI.get<Button>(START);
    /*
    PAUSE_MENU.add(START_MAIN, Button("Buttons", Vector2D(300, 100), Vector2D(200, 120)));
    Button* start_pause = PAUSE_MENU.get<Button>(START_MAIN);
    start_pause -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    PAUSE_MENU.add(OPTIONS_MAIN, Button("Buttons", Vector2D(300, 300), Vector2D(200, 120)));
    Button* options_pause = PAUSE_MENU.get<Button>(OPTIONS_MAIN);
    options_pause -> setButton(2);
    
    PAUSE_MENU.add(QUIT_MAIN, Button("Buttons", Vector2D(300, 500), Vector2D(200, 120)));
    Button* quit_pause = PAUSE_MENU.get<Button>(QUIT_MAIN);
    quit_pause -> setButton(3);
    quit_pause -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_EXIT;
    };
    */
}


void Menu::buttonSystem(SDL_Event &e)
{
    
    float x = 0.0f;
    float y = 0.0f;
    
    SDL_GetMouseState(&x, &y);
    auto view = MAIN_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];    // Get mouse location
        const float bx = button.x();
        const float by = button.y();
        const float bw = button.w();
        const float bh = button.h();
        
        const bool inside = (x >= bx) && (x <= bx + bw) && (y >= by) && (y <= by + bh); // Check if mouse is over button
        
        if (inside)
        {
            button.hovering(inside);
            if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)  // On button press
            {
                button.onClick();
                //CURRENT_STATE = STATE_GAMEPLAY;
                //std::cout << CURRENT_STATE << std::endl;
                
            }
        }
        else
        {
            button.hovering(inside);
        }
    }
}

void Menu::renderSystemMain(SDL_Renderer *renderer)
{
    if (!SDL_RenderClear(renderer))
    {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    }
    
    auto view = MAIN_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
    
    
    
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    
    SDL_RenderPresent(renderer);

}

void Menu::renderSystemPause(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    
    PAUSE_BACKGROUND = {0,0, GLOBALS::SCREEN_WIDTH, GLOBALS::SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &PAUSE_BACKGROUND);
    
    auto view = MAIN_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
}
