#include "MenuSystem.hpp"
#include "../Globals.hpp"

Menu::Menu()
{
    UI.add(START, Button("Buttons", Vector2D(300, 100), Vector2D(200, 120)));
    Button* start = UI.get<Button>(START);
    start -> onClick = []()
    {
        Globals::CURRENT_STATE = Globals::STATE_GAMEPLAY;
    };
    
    UI.add(OPTIONS, Button("Buttons", Vector2D(300, 300), Vector2D(200, 120)));
    Button* options = UI.get<Button>(OPTIONS);
    options -> setButton(2);
    
    UI.add(QUIT, Button("Buttons", Vector2D(300, 500), Vector2D(200, 120)));
    Button* quit = UI.get<Button>(QUIT);
    quit -> setButton(3);
    quit -> onClick = []()
    {
        Globals::CURRENT_STATE = Globals::STATE_EXIT;
    };
    //START_BUTTON = UI.get<Button>(START);
}


void Menu::buttonSystem(SDL_Event &e)
{
    
    float x = 0.0f;
    float y = 0.0f;
    
    SDL_GetMouseState(&x, &y);
    auto view = UI.all<Button>();
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

void Menu::renderSystem(SDL_Renderer *renderer)
{
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    if (!SDL_RenderClear(renderer))
    {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    }
    
    auto view = UI.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
    SDL_RenderPresent(renderer);

}
