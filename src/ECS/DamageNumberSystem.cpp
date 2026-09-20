//
//  DamageNumberSystem.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 9/19/26.
//

#include "DamageNumberSystem.hpp"
#include "../Globals.hpp"
#include "CollisionSystem.hpp"

bool DamageNumberSystem::init(TTF_TextEngine* engine, TTF_Font* font)
{
        if (!engine || !font)
            return false;

        ENGINE = engine;
        FONT = font;

        return true;
}

void DamageNumberSystem::spawn(int damage, const Vector2D& pos)
{
    std::string damageString = std::to_string(damage);

    TTF_Text* text = TTF_CreateText(ENGINE, FONT, damageString.c_str(), 0);

    if (!text)
    {
        return;
    }
    
    TTF_SetTextColor(text, 255, 80, 80, 255);
    
    Entity e = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(e, DamageNumber(text, pos));
}

void DamageNumberSystem::update(float deltaTime)
{
    auto view = GLOBALS::REGISTRY.all<DamageNumber>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        DamageNumber& damage_num = view.components[i];
        damage_num.age += deltaTime;
        damage_num.y += damage_num.velocityY * deltaTime;
        
        float remaining = 1.0f - (damage_num.age / damage_num.lifetime);

        if (remaining < 0.0f)
            remaining = 0.0f;

        Uint8 alpha = static_cast<Uint8>(255.0f * remaining);

        TTF_SetTextColor(damage_num.text, 223, 62, 35, alpha);
        
        if (damage_num.age >= damage_num.lifetime)
        {
            TTF_DestroyText(damage_num.text);
            Entity entity = view.entities[i];
            deadEntities().push_back(entity);
            
        }
    }
}

void DamageNumberSystem::draw(const Camera2D& camera)
{
    auto view = GLOBALS::REGISTRY.all<DamageNumber>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        DamageNumber& damage_num = view.components[i];
        Vector2D pos = camera.worldToScreen(Vector2D(damage_num.x, damage_num.y));
        TTF_DrawRendererText(damage_num.text, pos.x, pos.y);
    }
}

