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
public:
    void addXP(int xp, PlayerSystems& player);
    int getLevel();
    void XPCollisions(std::vector<CollisionEvent>& collisions, PlayerSystems& player);
    static void spawnXPDrop(const Vector2D& position);
};
#endif /* XPSystem_hpp */
