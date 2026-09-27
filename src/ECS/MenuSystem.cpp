#include "MenuSystem.hpp"
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
    
    PAUSE_MENU.add(CONTINUE_PAUSE, Button("MenuButtons", Vector2D(260, 68), SIZE_MENU));
    Button* continue_pause  = PAUSE_MENU.get<Button>(CONTINUE_PAUSE);
    continue_pause -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    PAUSE_MENU.add(RESTAR_PAUSET, Button("MenuButtons", Vector2D(260, 132), SIZE_MENU));
    Button* restart_pause = PAUSE_MENU.get<Button>(RESTAR_PAUSET);
    restart_pause -> setButton(4);
    restart_pause -> onClick = []()
    {
        Game::restart();
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    PAUSE_MENU.add(OPTIONS_PAUSE, Button("MenuButtons", Vector2D(260, 196), SIZE_MENU));
    Button* options_pause = PAUSE_MENU.get<Button>(OPTIONS_PAUSE);
    options_pause -> setButton(2);
    options_pause -> onClick = []()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_OPTIONS;
    };
    
    PAUSE_MENU.add(QUIT_PAUSE, Button("MenuButtons", Vector2D(260, 260), SIZE_MENU));
    Button* quit_pause = PAUSE_MENU.get<Button>(QUIT_PAUSE);
    quit_pause -> setButton(3);
    quit_pause -> onClick = []()
    {
        Game::restart();
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_MAIN_MENU;
    };
    
    
    OPTIONS_MENU.add(ALL_BUTTON, Button("Options", Vector2D(368, 64), SIZE_OPTION));
    Button* all_button = OPTIONS_MENU.get<Button>(ALL_BUTTON);
    all_button -> onClick = [this]()
    {
        //all_button -> setButton(ALL + 1);
        ALL = !ALL;
        //AudioManager::getInstance().setMasterVolume(1.0f * ALL);
        AudioManager::getInstance().setSoundVolume(1.0f * ALL);
        AudioManager::getInstance().setMusicVolume(1.0f * ALL);

    };
    
    OPTIONS_MENU.add(MUSIC_BUTTON, Button("Options", Vector2D(368, 128), SIZE_OPTION));
    Button* music_button = OPTIONS_MENU.get<Button>(MUSIC_BUTTON);
    music_button -> onClick = [this]()
    {
        //music_button -> setButton(1 + MUSIC);
        MUSIC = !MUSIC;
        AudioManager::getInstance().setMusicVolume(1.0f * MUSIC);
    };
    
    OPTIONS_MENU.add(SFX_BUTTON, Button("Options", Vector2D(368, 192), SIZE_OPTION));
    Button* sfx_button = OPTIONS_MENU.get<Button>(SFX_BUTTON);
    sfx_button -> onClick = [this]()
    {
        //sfx_button -> setButton(SFX + 1);
        SFX = !SFX;
        AudioManager::getInstance().setSoundVolume(1.0f * SFX);
        //AudioManager::getInstance().pauseAllSounds();
    };
    
    OPTIONS_MENU.add(BACK_BUTTON, Button("BackButton", Vector2D(368, 256), SIZE_OPTION));
    Button* back_button = OPTIONS_MENU.get<Button>(BACK_BUTTON);
    back_button -> onClick = [this]()
    {
        //sfx_button -> setButton(SFX + 1);
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_PAUSED;
    };

    GLOBALS::REGISTRY.add(PAUSE_BUTTON, Button("PauseButton", Vector2D(312, 5), Vector2D(16, 16)));
    Button* pause_button = GLOBALS::REGISTRY.get<Button>(PAUSE_BUTTON);
    pause_button -> onClick = [this]()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_PAUSED;
    };
    
    DEATH_MENU.add(RESTART_DEATH, Button("MenuButtons", Vector2D(260, 132), SIZE_MENU));
    Button* restart_death = DEATH_MENU.get<Button>(RESTART_DEATH);
    restart_death -> setButton(4);
    restart_death -> onClick = [this]()
    {
        Game::restart();
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_GAMEPLAY;
    };
    
    DEATH_MENU.add(QUIT_DEATH, Button("MenuButtons", Vector2D(260, 196), SIZE_MENU));
    Button* quit_death = DEATH_MENU.get<Button>(QUIT_DEATH);
    quit_death -> setButton(3);
    quit_death -> onClick = [this]()
    {
        GLOBALS::CURRENT_STATE = GLOBALS::STATE_MAIN_MENU;
    };
}

void Menu::initText(PlayerSystems& player)
{
    TextManager::createLabel("Start", "Buttons", "Start", COLORS::WHITE);
    TextManager::createLabel("Continue", "Buttons", "Continue", COLORS::WHITE);
    TextManager::createLabel("Options", "Buttons", "Options", COLORS::WHITE);
    TextManager::createLabel("Quit", "Buttons", "Quit", COLORS::WHITE);
    TextManager::createLabel("Restart", "Buttons", "Restart", COLORS::WHITE);
    TextManager::createLabel("All", "Buttons", "All", COLORS::WHITE);
    TextManager::createLabel("Music", "Buttons", "Music", COLORS::WHITE);
    TextManager::createLabel("SFX", "Buttons", "SFX", COLORS::WHITE);


    TextManager::createLabel("ROF", "Default",          "Increase Rate of \nFire by 30%", COLORS::WHITE);
    TextManager::createLabel("Speed", "Default",        "Increase Speed by \n30%", COLORS::WHITE);
    TextManager::createLabel("Damage", "Default",       "Increase Damage \nby 30%", COLORS::WHITE);
    TextManager::createLabel("Health", "Default",       "Increase Health \nby 30%", COLORS::WHITE);
    TextManager::createLabel("PSpeed", "Default",       "Increase \nProjectile\nSpeed by 30%", COLORS::WHITE);
    TextManager::createLabel("XPMutiplier", "Default",  "Increase XP gains \nby 30%", COLORS::WHITE);
    TextManager::createLabel("XPRange", "Default",      "Increase XP pick\nup range by 40%", COLORS::WHITE);
    TextManager::createLabel("Normal", "Default",       "Returns weapon \nback to normal. \nRaises weapon \nstats by 10%", COLORS::WHITE);
    TextManager::createLabel("Shotgun", "Default",      "Weapon becomes a \nshotgun.Raises \nweapon \nstats by 10%", COLORS::WHITE);
    TextManager::createLabel("SMG", "Default",          "Weapon becomes a \nSMG. Raises weapon stats \nby 10%", COLORS::WHITE);
    TextManager::createLabel("Railgun", "Default",      "Weapon becomes a \nRailgun. Raises \nweapon stats \nby 10%", COLORS::WHITE);
    TextManager::createLabel("Astroids", "Default",     "Astroids will orbit \nthe ship", COLORS::WHITE);
    TextManager::createLabel("Shield", "Default",       "Will give you a \nshield with a \ncooldown", COLORS::WHITE);
    TextManager::createLabel("Gunner", "Default",       "Gunner ship will \nbe at your \nside", COLORS::WHITE);
    TextManager::createLabel("PowerUpTIme", "Default",  "Power ups last \n30% longer", COLORS::WHITE);
    TextManager::createLabel("HealAmount", "Default",   "Heal 30% more \nevery level up", COLORS::WHITE);

    TextManager::createLabel("DoubleDamage", "Default", "Double Damage!", COLORS::YELLOW);
    TextManager::createLabel("FreezeTime", "Default", "Time Frozen!", COLORS::YELLOW);
    TextManager::createLabel("FreeLevel", "Default", "Free Level!", COLORS::YELLOW);
    TextManager::createLabel("DoubleXp", "Default", "Double XP!", COLORS::YELLOW);
    TextManager::createLabel("Invincible", "Default", "Invincibility!", COLORS::YELLOW);
    TextManager::createLabel("Heal", "Default", "Full Health!", COLORS::YELLOW);
    TextManager::createLabel("GrabXp", "Default", "Attract XP!", COLORS::YELLOW);

    
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

void Menu::buttonSystem(SDL_Event &e)   // Used to handle button inputs
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
                    button.hovering(inside + 1);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {
                        AudioManager::getInstance().playSound("Button");
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(inside + 1);
                }
            }
            break;
        }
        case GLOBALS::STATE_PAUSED:
        {
            auto view = PAUSE_MENU.all<Button>();
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
                    button.hovering(inside + 1);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {
                        AudioManager::getInstance().playSound("Button");
                        button.onClick();
                    }
                }
                else
                {
                    button.hovering(inside + 1);
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
                    button.hovering(inside);
                    if (e.type == SDL_EVENT_MOUSE_BUTTON_UP && e.button.button == SDL_BUTTON_LEFT)
                    {
                        AudioManager::getInstance().playSound("Button");
                        button.onClick();
                        button.onOff();
                    }
                }
                else
                {
                    button.hovering(inside);
                }
            }
            break;
        }
        case GLOBALS::STATE_GAMEPLAY:
        {
            auto view = GLOBALS::REGISTRY.all<Button>();
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
        case GLOBALS::STATE_DEATH:
        {
            auto view = DEATH_MENU.all<Button>();
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
    
    auto view = PAUSE_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
   
    
    TextManager::drawLabel("Continue", 265, 68);
    TextManager::drawLabel("Restart", 265, 132);
    TextManager::drawLabel("Options", 265, 196);
    TextManager::drawLabel("Quit", 265, 260);
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
    
    TextManager::drawLabel("All", 200, 64);
    TextManager::drawLabel("Music", 200, 128);
    TextManager::drawLabel("SFX", 200, 192);

}

void Menu::renderSystemGameplay(SDL_Renderer *renderer)
{
    auto view = GLOBALS::REGISTRY.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
    
    if (PlayerSystems::POWER_UP)
    {
        TextManager::drawLabel(PlayerSystems::getLabelPowerUp(), 200, 50);
    }
}

void Menu::renderSystemDeath(SDL_Renderer *renderer)
{
    if (!SDL_SetRenderDrawColor(renderer, 180, 32, 42, 150))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    
    PAUSE_BACKGROUND = {0,0, GLOBALS::SCREEN_WIDTH, GLOBALS::SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &PAUSE_BACKGROUND);
    
    auto view = DEATH_MENU.all<Button>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Button& button = view.components[i];
        button.draw();
        //SDL_Log("Working");
    }
   
    
    TextManager::drawLabel("Restart", 265, 132);
    TextManager::drawLabel("Quit", 265, 196);
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
        case GLOBALS::STATE_GAMEPLAY:
            renderSystemGameplay(renderer);
            break;
        case GLOBALS::STATE_DEATH:
            renderSystemDeath(renderer);
            break;
        default:
            break;
    }
}

