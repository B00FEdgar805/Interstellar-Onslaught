#ifndef Game_hpp
#define Game_hpp

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <random>
#include <chrono>


class Game
{
private:
    bool RUNNING;
    Uint64 FRAME_START;
    int FRAME_TIME;
    SDL_Window *WINDOW;
    float DELTA_TIME;
    
    // Timer
    Uint64 START_TIME = 0;
    Uint64 LAST_TIME = 0;
    Uint64 RoF = 1000;
    SDL_Event e;

    

public:
    Game();
    ~Game();
    void init(const char* title, int width, int height, bool fullscreen);
    int run();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool isRunning();
    static void UpdateFPSCounter(float deltaTime);
    static SDL_Renderer *RENDERER;
    
    enum GameState
    {
        STATE_MAIN_MENU,
        STATE_GAMEPLAY,
        STATE_PAUSED,
        STATE_EXIT
    };

    inline static GameState CURRENT_STATE = STATE_MAIN_MENU;
};

#endif /* Game_hpp */
