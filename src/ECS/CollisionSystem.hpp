#ifndef CollisionSystem_hpp
#define CollisionSystem_hpp

#include "Registry.hpp"
#include "Types.hpp"
#include "Math.hpp"
#include "SDL3/SDL.h"

struct CollisionEvent
{
    Entity a = 0;
    Entity b = 0;

    // Direction to move entity A out of entity B.
    Vector2D normal{0.0f, 0.0f};

    // How far A overlaps B.
    float depth = 0.0f;

    bool isTrigger = false;
};

void collisionSystem(std::vector<CollisionEvent>& outCollisions, bool resolveSolidCollisions = true);
std::vector<CollisionEvent> collisionSystem(bool resolveSolidCollisions = true);
inline std::vector<Entity>& deadEntities()  // Used to keep track of all dead entites in the game
{
    static std::vector<Entity> deadEntities;
    return deadEntities;
}


#endif /* CollisionSystem_hpp */
