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
#include "Components/Item.hpp"
#include "../AudioManager.hpp"

void XPSystem::addXP(int xp, PlayerSystems& player)
{
    CURRENT_XP += (xp * XP_MULTIPLIER);
    AudioManager::getInstance().playSound("Xp");
    if (CURRENT_XP >= LEVEL_UP_XP)
    {
        LEVEL++;
        std::string l = "Level: %i", LEVEL;
        TextManager::setLabelText("Level", l);
        AudioManager::getInstance().playSound("LevelUp");
        // level up function
        player.left = player.randomUpgrade();
        player.middle = player.randomUpgrade();
        player.right = player.randomUpgrade();

        GLOBALS::CURRENT_STATE = GLOBALS::STATE_UPGRADE;
        CURRENT_XP = CURRENT_XP - LEVEL_UP_XP;
        LEVEL_UP_XP *= 1.15;
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

void XPSystem::XPCollisions(std::vector<CollisionEvent> &collisions, PlayerSystems& player)
{
    for (const CollisionEvent& collision : collisions)
    {
        BoxCollider* a = GLOBALS::REGISTRY.get<BoxCollider>(collision.a);
        BoxCollider* b = GLOBALS::REGISTRY.get<BoxCollider>(collision.b);

        if (a == nullptr || b == nullptr)
        {
            continue;
        }
    
        if(collision.isTrigger && a->tag == "player" && b->tag == "xp")
        {
            addXP(10, player);
            deadEntities().push_back(collision.b);
        }
        else if(collision.isTrigger && a->tag == "xp" && b->tag == "player")
        {
            addXP(10, player);
            deadEntities().push_back(collision.a);
        }
        
    }
    
    if (free_level)
    {
        addXP(LEVEL_UP_XP, player);
        free_level = false;
    }
}


void XPSystem::spawnXPDrop(const Vector2D& position)
{
    Vector2D v1 , v2 = {5.0f, 5.0f};
    Vector2D offset = v1 - v2.scale(XP_GRAB_RANGE);
    offset += Vector2D(5.0f, 5.0f);
    Entity xp = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(xp, Transform(position));
    GLOBALS::REGISTRY.add(xp, Sprite("XP", Vector2D(5,5)));
    GLOBALS::REGISTRY.add(xp, BoxCollider(
        SIZE.scale(XP_GRAB_RANGE),
        offset.scale(0.5f),
        true,     // isTrigger
        false,     // isStatic
        "xp"
    ));
    GLOBALS::REGISTRY.add(xp, Item());
}

void XPSystem::renderXPBar(SDL_Renderer *renderer)
{
    float x = 20.0f;
    float y = 340.0f;
    float w = 600.0f;
    //SDL_Log("%f", w);
    float h = 6.0f;
    SDL_FRect bgRect = { x, y, w, h };
    SDL_SetRenderDrawColor(renderer, 179, 185, 209, 100);
    SDL_RenderFillRect(renderer, &bgRect);
    
    float targetPercentage = CURRENT_XP / LEVEL_UP_XP;
    const float lerpSpeed = 0.10f; // Adjust for smoothing
    this -> visualXPPercentage += (targetPercentage - this -> visualXPPercentage) * lerpSpeed;
    this -> visualXPPercentage = std::max(0.0f, std::min(this -> visualXPPercentage, 1.0f));
    float fgWidth = w * this -> visualXPPercentage;

    SDL_FRect fgRect = { x + 1, y + 1, fgWidth - 2, h - 2};
    if (fgRect.w < 0) fgRect.w = 0;
    
    
    SDL_SetRenderDrawColor(renderer, 32, 214, 199, 255);
  //  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderFillRect(renderer, &fgRect);
    TextManager::drawLabel("Level", x, y - 24);
}

float XPSystem::getLevelUpXP()
{
    return LEVEL_UP_XP;
}

void XPSystem::reset()
{
    auto view = GLOBALS::REGISTRY.all<Item>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        deadEntities().push_back(entity);
    }
    LEVEL = 1;
    LEVEL_UP_XP = 100;
    CURRENT_XP = 0;
    XP_MULTIPLIER = 1.0f;
    XP_GRAB_RANGE = 1.0f;
    visualXPPercentage = 0.0f;
}

