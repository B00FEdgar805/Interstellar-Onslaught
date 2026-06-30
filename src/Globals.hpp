#ifndef Globals_hpp
#define Globals_hpp

struct Globals
{
    enum GameState
    {
        STATE_MAIN_MENU,
        STATE_GAMEPLAY,
        STATE_PAUSED,
        STATE_EXIT
    };
    
    inline static GameState CURRENT_STATE = STATE_MAIN_MENU;
};
#endif /* Globals_hpp */
