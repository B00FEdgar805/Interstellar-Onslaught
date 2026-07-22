#include "MenuSystem.hpp"
#include "../Globals.hpp"
#include "../Game.hpp"
#include "../TextManager.hpp"
#include "XPSystem.hpp"

Menu::Menu()
{
    MAIN_MENU.add(START_MAIN, Button("MenuButtons", Vector2D(280, 100), SIZE_MENU));
    Button* start_main = MAIN_MENU.get<Button>(START_MAIN);
    start_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    MAIN_MENU.add(OPTIONS_MAIN, Button("MenuButtons", Vector2D(280, 164), SIZE_MENU));
    Button* options_main = MAIN_MENU.get<Button>(OPTIONS_MAIN);
    options_main -> setButton(2);
    
    MAIN_MENU.add(QUIT_MAIN, Button("MenuButtons", Vector2D(280, 228), SIZE_MENU));
    Button* quit_main = MAIN_MENU.get<Button>(QUIT_MAIN);
    quit_main -> setButton(3);
    quit_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_EXIT;
    };
    
    

}

void Menu::initText(Registry& registry, PlayerSystems& player)
{
    TextManager::createLabel("Start", "Default", "Start", COLORS::WHITE);
    TextManager::createLabel("Continue", "Default", "Continue", COLORS::WHITE);
    TextManager::createLabel("Options", "Default", "Options", COLORS::WHITE);
    TextManager::createLabel("Quit", "Default", "Quit", COLORS::WHITE);
    
    TextManager::createLabel("ROF", "Default", "Increase Rate of Fire by 10%", COLORS::WHITE);
    TextManager::createLabel("Speed", "Default", "Increase Speed by 10%", COLORS::WHITE);
    TextManager::createLabel("Damage", "Default", "Increase Damage by 10%", COLORS::WHITE);
    TextManager::createLabel("Health", "Default", "Increase Health by 10%", COLORS::WHITE);

    
    UPGRADE_MENU.add(UPGRADE_L, Button("UpgradeButton", Vector2D(10,80), SIZE_UPGRADE));
    Button* upgrade_l = UPGRADE_MENU.get<Button>(UPGRADE_L);
    upgrade_l -> onClick = [&player, &registry]()
    {
        //SDL_Log("Left");
        player.upgrade(PlayerSystems::left, registry);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    UPGRADE_MENU.add(UPGRADE_M, Button("UpgradeButton", Vector2D(220,80), SIZE_UPGRADE));
    Button* upgrade_m = UPGRADE_MENU.get<Button>(UPGRADE_M);
    upgrade_m -> onClick = [&player, &registry]()
    {
        //SDL_Log("Middle");
        player.upgrade(PlayerSystems::middle, registry);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    UPGRADE_MENU.add(UPGRADE_R, Button("UpgradeButton", Vector2D(430,80), SIZE_UPGRADE));
    Button* upgrade_r = UPGRADE_MENU.get<Button>(UPGRADE_R);
    upgrade_r -> onClick = [&player, &registry]()
    {
        //SDL_Log("Right");
        player.upgrade(PlayerSystems::right, registry);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
}

void Menu::buttonSystem(SDL_Event &e)
{
    float windowX = 0.0f;
    float windowY = 0.0f;

    // Get mouse in window coordinates
    SDL_GetMouseState(&windowX, &windowY);

    // Convert window coordinates to logical render coordinates (account for letterboxing)
    float logicalX = windowX;
    float logicalY = windowY;

    SDL_Window* window = SDL_GetRenderWindow(Game::RENDERER);
    if (window)
    {
        int winW = 0;
        int winH = 0;
        SDL_GetWindowSize(window, &winW, &winH);

        const float logicalW = static_cast<float>(GLOBALS::SCREEN_WIDTH);
        const float logicalH = static_cast<float>(GLOBALS::SCREEN_HEIGHT);

        // Compute scale used by SDL_LOGICAL_PRESENTATION_LETTERBOX
        const float scaleX = (logicalW > 0.0f) ? (static_cast<float>(winW) / logicalW) : 1.0f;
        const float scaleY = (logicalH > 0.0f) ? (static_cast<float>(winH) / logicalH) : 1.0f;
        const float scale = (scaleX < scaleY) ? scaleX : scaleY;

        const float offsetX = (static_cast<float>(winW) - logicalW * scale) * 0.5f;
        const float offsetY = (static_cast<float>(winH) - logicalH * scale) * 0.5f;

        logicalX = (windowX - offsetX) / scale;
        logicalY = (windowY - offsetY) / scale;
    }
    // Select and iterate the appropriate button view based on current state without copying DenseView
    switch (GLOBALS::CURRENT_STATE)
    {
        case GLOBALS::STATE_MAIN_MENU:
        case GLOBALS::STATE_PAUSED:
        {
            auto view = MAIN_MENU.all<Button>();
            for (size_t i = 0; i < view.entities.size(); ++i)
            {
                Button& button = view.components[i];
                const float bx = button.x();
                const float by = button.y();
                const float bw = button.w();
                const float bh = button.h();

                const bool inside = (logicalX >= bx) && (logicalX <= bx + bw) && (logicalY >= by) && (logicalY <= by + bh);

                if (inside)
                {
                    button.hovering(inside);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(inside);
                }
            }
            break;
        }
        case GLOBALS::STATE_UPGRADE:
        {
            auto view = UPGRADE_MENU.all<Button>();
            for (size_t i = 0; i < view.entities.size(); ++i)
            {
                Button& button = view.components[i];
                const float bx = button.x();
                const float by = button.y();
                const float bw = button.w();
                const float bh = button.h();

                const bool inside = (logicalX >= bx) && (logicalX <= bx + bw) && (logicalY >= by) && (logicalY <= by + bh);

                if (inside)
                {
                    button.hovering(inside);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(inside);
                }
            }
            break;
        }
        default:
        {
            auto view = MAIN_MENU.all<Button>();
            for (size_t i = 0; i < view.entities.size(); ++i)
            {
                Button& button = view.components[i];
                const float bx = button.x();
                const float by = button.y();
                const float bw = button.w();
                const float bh = button.h();

                const bool inside = (logicalX >= bx) && (logicalX <= bx + bw) && (logicalY >= by) && (logicalY <= by + bh);

                if (inside)
                {
                    button.hovering(inside);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(inside);
                }
            }
            break;
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
    
    TextManager::drawLabel("Start", 290, 100);
    TextManager::drawLabel("Options", 290, 164);
    TextManager::drawLabel("Quit", 290, 228);
    
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
    
    TextManager::drawLabel("Continue", 285, 100);
    TextManager::drawLabel("Options", 285, 164);
    TextManager::drawLabel("Quit", 285, 228);
}

void Menu::renderSystemUpgrade(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    
    PAUSE_BACKGROUND = {0,0, GLOBALS::SCREEN_WIDTH, GLOBALS::SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &PAUSE_BACKGROUND);
    
    auto view = UPGRADE_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
    
    TextManager::drawLabel(PlayerSystems::getLabel(PlayerSystems::left), 10, 80);
    TextManager::drawLabel(PlayerSystems::getLabel(PlayerSystems::middle), 220, 80);
    TextManager::drawLabel(PlayerSystems::getLabel(PlayerSystems::right), 430, 80);
}

void Menu::renderUI(SDL_Renderer *renderer)
{
    switch (GLOBALS::CURRENT_STATE)
    {
        case GLOBALS::STATE_PAUSED:
            renderSystemPause(renderer);
            break;
        case GLOBALS::STATE_UPGRADE:
            renderSystemUpgrade(renderer);
            break;
        default:
            break;
    }
}


