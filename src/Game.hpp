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
    bool IMGUI = false;
    Uint64 FRAME_START;
    int FRAME_TIME;
    SDL_Window *WINDOW;
    float DELTA_TIME;
    float FPS = 0.0f;
    // Timer
    Uint64 START_TIME = 0;
    Uint64 LAST_TIME = 0;
    //Uint64 RoF = 1000;
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
    void mainLoopTick();
    static void restart();
    bool isRunning();
    void initIMGUI();
    float UpdateFPSCounter(float deltaTime);
    static inline SDL_Renderer *RENDERER = nullptr;
    
    inline static double elampsed_time = 0;

};

#endif /* Game_hpp */
