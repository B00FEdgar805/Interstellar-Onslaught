//
//  Game.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 2/25/26.
//

#include "Game.hpp"
#include "TextureManager.hpp"
#include "GameObject.hpp"

GameObject* player;

Game::Game()
{
    init("SDL Game", 800, 600, false);
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
    
    RUNNING = true;
    
    player = new GameObject("Assets/Spaceship.png", RENDERER, 0, 0);
    
}

int Game::run()
{
    while (isRunning())
    {
        FRAME_START = SDL_GetTicks();
        handleEvents();
        render();
        update();
        
        FRAME_TIME = SDL_GetTicks() - FRAME_START;
        
        if(FRAME_DELAY > FRAME_TIME)
        {
            SDL_Delay(FRAME_DELAY - FRAME_TIME);
        }
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
}

void Game::update()
{
    player -> update();
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
    
    player -> render();

    SDL_RenderPresent(RENDERER);
}

void Game::clean()
{
    // Cleanup
    SDL_DestroyRenderer(RENDERER);
    SDL_DestroyWindow(WINDOW);
    SDL_Quit();
}

bool Game::isRunning()
{
    return RUNNING;
}
