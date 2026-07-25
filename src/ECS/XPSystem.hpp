//
//  XPSystem.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 7/18/26.
//

#ifndef XPSystem_hpp
#define XPSystem_hpp


#include "Registry.hpp"
#include "CollisionSystem.hpp"
#include "PlayerSystems.hpp"


class XPSystem
{
private:
    unsigned int LEVEL = 1;
    unsigned int LEVEL_UP_XP = 10;
    unsigned int CURRENT_XP = 0;
    Vector2D SIZE = {5.0f, 5.0f};
public:
    void addXP(int xp, PlayerSystems& player);
    int getLevel();
    void XPCollisions(std::vector<CollisionEvent>& collisions, PlayerSystems& player);
    void spawnXPDrop(const Vector2D& position);
    inline static float XP_MULTIPLIER = 1.0f;
    inline static float XP_GRAB_RANGE = 1.0f;
};
#endif /* XPSystem_hpp */
