//
//  XPSystem.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 7/18/26.
//
#include "../Globals.hpp"
#include "XPSystem.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"

void XPSystem::addXP(int xp, PlayerSystems& player)
{
    CURRENT_XP += xp;
    if (CURRENT_XP >= LEVEL_UP_XP)
    {
        LEVEL++;
        // level up function
        player.left = player.randomUpgrade();
        player.middle = player.randomUpgrade();
        player.right = player.randomUpgrade();

        GLOBALS::CURRENT_STATE = GLOBALS::STATE_UPGRADE;
        LEVEL_UP_XP *= 1.10;
        CURRENT_XP = CURRENT_XP - LEVEL_UP_XP;
        //std::cout << PlayerSystems::left;
        //std::cout << PlayerSystems::middle;
        //std::cout << PlayerSystems::right;

        //SDL_Log("%i", LEVEL);
        
    }
}

int XPSystem::getLevel()
{
    return LEVEL;
}

void XPSystem::XPCollisions(Registry &registry, std::vector<CollisionEvent> &collisions, PlayerSystems& player)
{
    for (const CollisionEvent& collision : collisions)
    {
        BoxCollider* a = registry.get<BoxCollider>(collision.a);
        BoxCollider* b = registry.get<BoxCollider>(collision.b);

        if (a == nullptr || b == nullptr)
        {
            continue;
        }
    
        if(collision.isTrigger && a->tag == "player" && b->tag == "xp")
        {
            addXP(5, player);
            deadEntities().push_back(collision.b);
        }
        else if(collision.isTrigger && a->tag == "xp" && b->tag == "player")
        {
            addXP(5, player);
            deadEntities().push_back(collision.a);
        }
        
    }
}

void XPSystem::spawnXPDrop(Registry &registry, const Vector2D& position)
{
    Entity xp = registry.create();
    registry.add(xp, Transform(position));
    registry.add(xp, Sprite("XP", Vector2D(5,5)));
    registry.add(xp, BoxCollider(
        Vector2D(5.0f, 5.0f).scale(1),
        Vector2D(0.0f, 0.0f),
        true,     // isTrigger
        false,     // isStatic
        "xp"
    ));
}
