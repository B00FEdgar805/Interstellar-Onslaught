//
//  Game.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 2/25/26.
//

#include "Game.hpp"
#include "TextureManager.hpp"
#include "Map.hpp"
//#include "ECS/Components/Component.hpp"
#include "ECS/Components/Transform.hpp"
#include "ECS/Components/Sprite.hpp"
#include "ECS/Systems.hpp"
#include "ECS/Registry.hpp"



SDL_Renderer* Game::RENDERER = nullptr;
Map* map;
Registry registry;
Entity player = registry.create();;
Systems systems;


Game::Game()
{
    init("SDL Game", 800, 640, false);
}

Game::~Game()
{
    clean();
}

void Game::init(const char* title, int width, int height, bool fullscreen)
{
    int flags = 0;
    
    if(fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    else
    {
        flags = SDL_WINDOW_RESIZABLE;
    }
    
    // Initialize SDL (video + events)
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s\n", SDL_GetError());
        SDL_Quit();
    }
    
    // Create a window
    WINDOW = SDL_CreateWindow(title, width, height, flags);
    if (!WINDOW)
    {
        SDL_Log("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
    }
    
    // Create a renderer
    RENDERER = SDL_CreateRenderer(WINDOW, nullptr);
    if (!RENDERER)
    {
        SDL_Log("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(WINDOW);
        SDL_Quit();
    }
    
    SDL_SetRenderLogicalPresentation(RENDERER, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
    
    RUNNING = true;
    
    map = new Map();
    
    TextureManager::loadTexture("player", "Assets/Spaceship.png");
    registry.add(player, Sprite("player"));
    registry.add(player, Transform(100.0f, 100.0f));
    //registry.add(player, Transform(0,0));


    
   // Player.addComponent<PositionComponenet>();
    //Player.addComponent<SpriteComponent>("Assets/Spaceship.png");
}

int Game::run()
{
    auto lastTime = std::chrono::steady_clock::now();

    while (isRunning())
    {
        auto currentTime = std::chrono::steady_clock::now();

        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();

        lastTime = currentTime;

        if (deltaTime > 0.05f)
        {
            deltaTime = 0.05f;
        }
        
        handleEvents();
        render();
        update();
    }
    
    clean();
    return 0;
}


void Game::handleEvents()
{
    SDL_Event e;
    SDL_PollEvent(&e);
    switch (e.type)
    {
        case SDL_EVENT_QUIT:
            RUNNING = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            if (e.key.key == SDLK_ESCAPE)
            {
                RUNNING = false;
            }
            break;
        default:
            break;
            
    }
    
    //player movemnt
}

void Game::update()
{
    //manager.refresh();
    //manager.update();
    //movement system
    
}

void Game::render()
{
    if (!SDL_SetRenderDrawColor(RENDERER, 0, 0, 0, 0))
    {
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    if (!SDL_RenderClear(RENDERER))
    {
        SDL_Log("SDL_RenderClear failed: %s\n", SDL_GetError());
    }
    
    map -> drawMap();
    systems.renderSystem(registry, RENDERER);

    SDL_RenderPresent(RENDERER);
}

void Game::clean()
{
    // Cleanup
    TextureManager::clear();
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
}

bool Game::isRunning()
{
    return RUNNING;
}
