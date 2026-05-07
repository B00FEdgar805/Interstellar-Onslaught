//
//  Game.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 2/25/26.
//

#include "Game.hpp"

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
    
    
}

int Game::run()
{
    while (isRunning())
    {
        handleEvents();
        render();
        update();
        
        SDL_Delay(10);
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

    // Draw a simple rectangle to verify rendering
    SDL_FRect rect{ 100.0f, 100.0f, 200.0f, 150.0f };
    if (!SDL_SetRenderDrawColor(RENDERER, 255, 255, 255, 255))
    { // White
        SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
    }
    if (!SDL_RenderFillRect(RENDERER, &rect))
    {
        SDL_Log("SDL_RenderFillRect failed: %s\n", SDL_GetError());
    }

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
