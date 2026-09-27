#ifndef Globals_hpp
#define Globals_hpp

#include "ECS/Registry.hpp"
#include <chrono>
#include <iostream>

struct GLOBALS
{
    enum GameState
    {
        STATE_MAIN_MENU,
        STATE_GAMEPLAY,
        STATE_PAUSED,
        STATE_UPGRADE,
        STATE_OPTIONS,
        STATE_DEATH,
        STATE_EXIT
    };
    
    inline static GameState CURRENT_STATE = STATE_MAIN_MENU;
    // 640 by 360
    static constexpr int SCREEN_HEIGHT = 360; //640;
    static constexpr int SCREEN_WIDTH = 640; //800;
    // World size 1600 x 1280
    //
    inline static bool COLLISION_BOXES = false;
    inline static bool INVINCIBLE = false;
    inline static bool INVINCIBLE_CONSTANT = false;
    inline static bool FREEZE = false;
    
    inline static Registry REGISTRY;
    //inline static Entity PLAYER;
};


class TIMER_DEBUG
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> START_TIME_POINT;
public:
    TIMER_DEBUG()
    {
        START_TIME_POINT = std::chrono::high_resolution_clock::now();
    }
    
    ~TIMER_DEBUG()
    {
        stop();
    }
    
    void stop()
    {
        auto endTimePoint = std::chrono::high_resolution_clock::now();
        
        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(START_TIME_POINT).time_since_epoch();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch();
        
        auto duration = (end - start).count();
        double ms = duration * 0.001;
        std::cout << duration << " ns ";
        std::cout << ms << " ms" << std::endl;
    }
};

#endif /* Globals_hpp */

