//
//  Game.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 2/25/26.
//

#ifndef Game_hpp
#define Game_hpp

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Game
{
private:
    bool RUNNING;
    SDL_Window *WINDOW;
    SDL_Renderer *RENDERER;
    
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
    
};

#endif /* Game_hpp */
