#include "MenuSystem.hpp"
#include "../Globals.hpp"
#include "../Game.hpp"
#include "../TextManager.hpp"
#include "XPSystem.hpp"
#include "../AudioManager.hpp"

Menu::Menu()
{
    MAIN_MENU.add(START_MAIN, Button("MenuButtons", Vector2D(260, 100), SIZE_MENU));
    Button* start_main = MAIN_MENU.get<Button>(START_MAIN);
    start_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    MAIN_MENU.add(OPTIONS_MAIN, Button("MenuButtons", Vector2D(260, 164), SIZE_MENU));
    Button* options_main = MAIN_MENU.get<Button>(OPTIONS_MAIN);
    options_main -> setButton(2);
    options_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_OPTIONS;
    };
    
    MAIN_MENU.add(QUIT_MAIN, Button("MenuButtons", Vector2D(260, 228), SIZE_MENU));
    Button* quit_main = MAIN_MENU.get<Button>(QUIT_MAIN);
    quit_main -> setButton(3);
    quit_main -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_EXIT;
    };
    
    OPTIONS_MENU.add(ALL_BUTTON, Button("Options", Vector2D(368, 64), SIZE_OPTION));
    Button* all_button = OPTIONS_MENU.get<Button>(ALL_BUTTON);
    all_button -> onClick = [this]()
    {
        //all_button -> setButton(ALL + 1);
        ALL = !ALL;
        AudioManager::getInstance().setMasterVolume(1.0f * ALL);
    };
    
    OPTIONS_MENU.add(MUSIC_BUTTON, Button("Options", Vector2D(368, 128), SIZE_OPTION));
    Button* music_button = OPTIONS_MENU.get<Button>(MUSIC_BUTTON);
    music_button -> onClick = [this]()
    {
        //music_button -> setButton(1 + MUSIC);
        MUSIC = !MUSIC;
        AudioManager::getInstance().setMusicVolume(0.2f * MUSIC);
    };
    
    OPTIONS_MENU.add(SFX_BUTTON, Button("Options", Vector2D(368, 192), SIZE_OPTION));
    Button* sfx_button = OPTIONS_MENU.get<Button>(SFX_BUTTON);
    sfx_button -> onClick = [this]()
    {
        //sfx_button -> setButton(SFX + 1);
        SFX = !SFX;
        AudioManager::getInstance().setSoundVolume(1.0f * SFX);
    };
    
    OPTIONS_MENU.add(BACK_BUTTON, Button("Options", Vector2D(368, 256), SIZE_OPTION));
    Button* back_button = OPTIONS_MENU.get<Button>(BACK_BUTTON);
    back_button -> onClick = [this]()
    {
        //sfx_button -> setButton(SFX + 1);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_PAUSED;
    };

}

void Menu::initText(PlayerSystems& player)
{
    TextManager::createLabel("Start", "Buttons", "Start", COLORS::WHITE);
    TextManager::createLabel("Continue", "Buttons", "Continue", COLORS::WHITE);
    TextManager::createLabel("Options", "Buttons", "Options", COLORS::WHITE);
    TextManager::createLabel("Quit", "Buttons", "Quit", COLORS::WHITE);
    
    TextManager::createLabel("ROF", "Default",          "Increase Rate of \nFire by 10%", COLORS::WHITE);
    TextManager::createLabel("Speed", "Default",        "Increase Speed by \n 10%", COLORS::WHITE);
    TextManager::createLabel("Damage", "Default",       "Increase Damage \nby 10%", COLORS::WHITE);
    TextManager::createLabel("Health", "Default",       "Increase Health \nby 10%", COLORS::WHITE);
    TextManager::createLabel("PSpeed", "Default",       "Increase Projectile\n Speed by 10%", COLORS::WHITE);
    TextManager::createLabel("XPMutiplier", "Default",  "Increase XP gains \n by 10%", COLORS::WHITE);
    TextManager::createLabel("XPRange", "Default",      "Increase XP pick\n up range by 20%", COLORS::WHITE);
    TextManager::createLabel("Normal", "Default",       "Returns weapon back\n to normal. Raises\n weapon stats by 10%", COLORS::WHITE);
    TextManager::createLabel("Shotgun", "Default",      "Weapon becomes a \nshotgun.\n Raises weapon \nstats by 10%", COLORS::WHITE);
    TextManager::createLabel("SMG", "Default",          "Weapon becomes a SMG.\n Raises weapon stats \nby 10%", COLORS::WHITE);
    TextManager::createLabel("Railgun", "Default",      "Weapon becomes a \nRailgun. Raises weapon \nstats by 10%", COLORS::WHITE);
    TextManager::createLabel("Astroids", "Default",     "Astroids will orbit \nthe ship", COLORS::WHITE);
    TextManager::createLabel("Shield", "Default",       "Will give you a shield\n with a \ncooldown", COLORS::WHITE);
    TextManager::createLabel("Gunner", "Default",       "Gunner ship will be at\n your side", COLORS::WHITE);
    TextManager::createLabel("PowerUpTIme", "Default",  "Power ups last \n30% longer", COLORS::WHITE);
    
    UPGRADE_MENU.add(UPGRADE_L, Button("UpgradeButton", Vector2D(10,80), SIZE_UPGRADE));
    Button* upgrade_l = UPGRADE_MENU.get<Button>(UPGRADE_L);
    upgrade_l -> onClick = [&player]()
    {
        //SDL_Log("Left");
        player.upgrade(PlayerSystems::left);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    UPGRADE_MENU.add(UPGRADE_M, Button("UpgradeButton", Vector2D(220,80), SIZE_UPGRADE));
    Button* upgrade_m = UPGRADE_MENU.get<Button>(UPGRADE_M);
    upgrade_m -> onClick = [&player]()
    {
        //SDL_Log("Middle");
        player.upgrade(PlayerSystems::middle);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    UPGRADE_MENU.add(UPGRADE_R, Button("UpgradeButton", Vector2D(430,80), SIZE_UPGRADE));
    Button* upgrade_r = UPGRADE_MENU.get<Button>(UPGRADE_R);
    upgrade_r -> onClick = [&player]()
    {
        //SDL_Log("Right");
        player.upgrade(PlayerSystems::right);
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
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {
                        AudioManager::getInstance().playSound("Button");
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
                    button.hovering(true);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {
                        AudioManager::getInstance().playSound("Button");
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(false);
                }
            }
            break;
        }
        case GLOBALS::STATE_OPTIONS:
        {
            auto view = OPTIONS_MENU.all<Button>();
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
                    button.hovering(inside - 1);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {
                        AudioManager::getInstance().playSound("Button");
                        button.onClick();
                        button.onOff();
                    }
                }
                else
                {
                    button.hovering(inside - 1);
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
                    button.hovering(true);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {                        AudioManager::getInstance().playSound("Button");
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(false);
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
    
    TextManager::drawLabel("Start", 270, 100);
    TextManager::drawLabel("Options", 270, 164);
    TextManager::drawLabel("Quit", 270, 228);
    
    SDL_RenderPresent(renderer);

}

void Menu::renderSystemPause(SDL_Renderer *renderer)
{
    
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
   
    
    TextManager::drawLabel("Continue", 265, 100);
    TextManager::drawLabel("Options", 265, 164);
    TextManager::drawLabel("Quit", 265, 228);
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
    
    TextManager::drawLabel(PlayerSystems::getLabel(PlayerSystems::left), 18, 84);
    TextManager::drawLabel(PlayerSystems::getLabel(PlayerSystems::middle), 228, 84);
    TextManager::drawLabel(PlayerSystems::getLabel(PlayerSystems::right), 438, 84);
}

void Menu::renderSystemOptions(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    if (!SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    
    PAUSE_BACKGROUND = {0,0, GLOBALS::SCREEN_WIDTH, GLOBALS::SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &PAUSE_BACKGROUND);
    
    auto view = OPTIONS_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
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
        case GLOBALS::STATE_OPTIONS:
            renderSystemOptions(renderer);
            break;
        default:
            break;
    }
}

