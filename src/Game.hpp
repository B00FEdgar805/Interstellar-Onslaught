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
    
};

#endif /* Game_hpp */
