#ifndef Globals_hpp
#define Globals_hpp

struct GLOBALS
{
    enum GameState
    {
        STATE_MAIN_MENU,
        STATE_GAMEPLAY,
        STATE_PAUSED,
        STATE_EXIT
    };
    
    inline static GameState CURRENT_STATE = STATE_MAIN_MENU;
    static const int SCREEN_HEIGHT = 640;
    static const int SCREEN_WIDTH = 800;
    //static float DELTA_TIME;
    
};
#endif /* Globals_hpp */
