#ifndef Globals_hpp
#define Globals_hpp

#include "ECS/Registry.hpp"

struct GLOBALS
{
    enum GameState
    {
        STATE_MAIN_MENU,
        STATE_GAMEPLAY,
        STATE_PAUSED,
        STATE_UPGRADE,
        STATE_EXIT
    };
    
    inline static GameState CURRENT_STATE = STATE_MAIN_MENU;
    // 640 by 360
    static constexpr int SCREEN_HEIGHT = 360; //640;
    static constexpr int SCREEN_WIDTH = 640; //800;
    inline static bool COLLISION_BOXES = false;
    inline static bool INVINCIBLE = false;

    
    inline static Registry REGISTRY;
    //inline static Entity PLAYER;
};


#endif /* Globals_hpp */

