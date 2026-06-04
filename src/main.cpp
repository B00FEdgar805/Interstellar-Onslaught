/*
#include "Game.hpp"

int main(int argc, char* argv[])
{
    Game game;
    game.run();
}
*/

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Game.hpp"

// Global pointer to your Game object context
Game* g_game = nullptr;

// 1. INITIALIZATION: Called once when the application boots
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    g_game = new Game();
    if (!g_game->    init("SDL Game", 800, 640, false)) {
        return SDL_APP_FAILURE; // Shuts down immediately if initialization fails
    }
    return SDL_APP_CONTINUE; // Signals SDL to start the game loop
}

// 2. THE MAIN LOOP STEP: Called continuously by macOS at the ideal refresh rate
SDL_AppResult SDL_AppIterate(void *appstate)
{
    // High-precision tracking for DELTA_TIME
    static uint64_t lastTimeNS = SDL_GetTicksNS();
    uint64_t currentTimeNS = SDL_GetTicksNS();
    
    g_game->DELTA_TIME = (float)(currentTimeNS - lastTimeNS) / 1000000000.0f;
    lastTimeNS = currentTimeNS;

    if (g_game->DELTA_TIME > 0.05f) g_game->DELTA_TIME = 0.05f;

    // Execute your engine loops
    g_game->update();
    g_game->render(); // Put back your original tilemap/render systems!

    if (!g_game->isRunning()) {
        return SDL_APP_SUCCESS; // Safely exits the game
    }
    return SDL_APP_CONTINUE; // Tells macOS to cleanly process the next frame slot
}

// 3. EVENT PROCESSING: Handles OS window messages automatically
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE) {
        return SDL_APP_SUCCESS;
    }

    // Pass any unhandled input structures down to your entity input registry
    g_game->handleSingleEvent(event);
    return SDL_APP_CONTINUE;
}

// 4. CLEANUP: Called once when exiting
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    if (g_game) {
        g_game->clean();
        delete g_game;
    }
}


