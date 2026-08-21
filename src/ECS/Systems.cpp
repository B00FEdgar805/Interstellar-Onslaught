#include "Systems.hpp"
#include "Components/PlayerControl.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Animation.hpp"
#include "Components/Projectile.hpp"
#include "Components/Enemy.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Health.hpp"
#include "../Globals.hpp"

void Systems::playerInputSystem()
{
    const bool* keys = SDL_GetKeyboardState(nullptr);
    
    auto view = GLOBALS::REGISTRY.all<PlayerControl>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Velocity* velocity = GLOBALS::REGISTRY.get<Velocity>(entity);

        if (!velocity)
        {
            continue;
        }

        //constexpr float speed = 220.0f;

        velocity -> value = Vector2D(0.0f, 0.0f);

        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        {
            velocity -> value.x -= 1.0f;
            angle = 270.0f;
        }

        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        {
            velocity -> value.x += 1.0f;
            angle = 90.0f;
        }

        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        {
            velocity -> value.y -= 1.0f;
            angle = 0.0f;
        }

        if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        {
            velocity -> value.y += 1.0f;
            angle = 180.0f;
        }
        
        if (velocity -> value.y != 0 && velocity -> value.x != 0)
        {
            velocity -> value.normalize();
            if (velocity -> value.y < 0 && velocity -> value.x < 0)
            {
                angle = 315.0f;
            }
            else if (velocity -> value.y > 0 && velocity -> value.x < 0)
            {
                angle = 225.0f;
            }
            else if (velocity -> value.y > 0 && velocity -> value.x > 0)
            {
                angle = 135.0f;
            }
            else if (velocity -> value.y < 0 && velocity -> value.x > 0)
            {
                angle = 45.0f;
            }
        }
        
        if (velocity -> value.y != 0 || velocity -> value.x != 0)   // used to store last direction of player
        {
            velocity -> direction = velocity -> value;
        }
        
        //velocity -> value.scale(speed);
       
        //SDL_Log("X %f", velocity -> value.x);
        //SDL_Log("Y %f", velocity -> value.y);

    }
}




void Systems::movementSystem(float delta_time)
{
    auto view = GLOBALS::REGISTRY.all<Velocity>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Velocity& velocity = view.components[i];

        Transform* transform = GLOBALS::REGISTRY.get<Transform>(entity);
        //velocity.value.scale(velocity.m_speed);
        
        if (!transform)
        {
            continue;
        }
        
        transform -> position += velocity.value.scale(delta_time * velocity.m_speed);
        
       // Vector2D delta = velocity.value;
        //delta.scale(delta_time * velocity.m_speed);
        //transform->position += delta;
        
        //SDL_Log("X: %f", velocity.value.x);
        //SDL_Log("Y: %f", velocity.value.y);
        
        //SDL_Log("X: %f", transform -> position.x);
        //SDL_Log("Y: %f", transform -> position.y);

        
    }
}

void Systems::renderSystem(SDL_Renderer* renderer, const Camera2D& camera)  // Renders every entity that has a sprite
{
    auto view = GLOBALS::REGISTRY.all<Sprite>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Sprite& sprite = view.components[i];

        Transform* transform = GLOBALS::REGISTRY.get<Transform>(entity);
        //Animation* animation = GLOBALS::REGISTRY.get<Animation>(entity);
        //PlayerControl* controlled = GLOBALS::REGISTRY.get<PlayerControl>(entity);
        //Projectile* projectile = GLOBALS::REGISTRY.get<Projectile>(entity);
        
        
        if (!transform)
        {
            continue;
        }
        
        SDL_FRect worldDestination;
        worldDestination.x = transform -> position.x;
        worldDestination.y = transform -> position.y;
        worldDestination.w = sprite.w();
        worldDestination.h = sprite.h();
        
        SDL_FRect screenDestination = camera.worldToScreenRect(worldDestination);

        //sprite.x(transform -> position.x);
        //sprite.y(transform -> position.y);
        
       // sprite.setPosition(transform -> position);
        
        
        sprite.setRect(screenDestination);
        
        //SDL_Log("%f", transform -> x);
        //SDL_Log("%f", transform -> y);
        
        
        
        if (GLOBALS::CURRENT_STATE == GLOBALS::STATE_PAUSED || GLOBALS::CURRENT_STATE == GLOBALS::STATE_UPGRADE)
        {
            if (GLOBALS::REGISTRY.has<Animation>(entity))
            {
                Animation* animation = GLOBALS::REGISTRY.get<Animation>(entity);
                sprite.Animate(0, animation -> speed, animation -> frames);
            }
        }
        else
        {
            if (GLOBALS::REGISTRY.has<Animation>(entity))
            {
                Animation* animation = GLOBALS::REGISTRY.get<Animation>(entity);
                sprite.Animate(SDL_GetTicks(), animation -> speed, animation -> frames);
            }
        }
        
        //SDL_Log("Working");
        
        if(GLOBALS::REGISTRY.has<PlayerControl>(entity) || sprite.getTextureID() == "Gunner")
        {
            sprite.draw(angle);
        }
        else if (GLOBALS::REGISTRY.has<Projectile>(entity) || GLOBALS::REGISTRY.has<Enemy>(entity))
        {
            Velocity* v = GLOBALS::REGISTRY.get<Velocity>(entity);
            sprite.draw(v -> directionToDegrees());
        }
        else
        {
            sprite.draw();
        }
        
        if (GLOBALS::REGISTRY.has<Health>(entity))  // Render Health bars
        {
            Health* health = GLOBALS::REGISTRY.get<Health>(entity);
            float x = screenDestination.x;
            float y = screenDestination.y + sprite.h() + 4;
            float w = sprite.w();
            //SDL_Log("%f", w);
            float h = 4.0f;
            SDL_FRect bgRect = { x, y, w, h };
            SDL_SetRenderDrawColor(renderer, 179, 185, 209, 255);
            if (GLOBALS::REGISTRY.has<PlayerControl>(entity)) // Player has backgound for health bar
            {
                SDL_RenderFillRect(renderer, &bgRect);
            }
            
            float percentage = health -> getHealth() / health -> getMaxHealth();
            percentage = std::max(0.0f, std::min(percentage, 1.0f));

            float fgWidth = w * percentage;

            SDL_FRect fgRect = { x + 1, y + 1, fgWidth - 2, h - 2};
            if (fgRect.w < 0) fgRect.w = 0;
            
            
            SDL_SetRenderDrawColor(renderer, 180, 32, 42, 255);
            SDL_RenderFillRect(renderer, &fgRect);
        }
        
        if (GLOBALS::COLLISION_BOXES && GLOBALS::REGISTRY.has<BoxCollider>(entity))
        {
            BoxCollider* b = GLOBALS::REGISTRY.get<BoxCollider>(entity);
            Vector2D position = {screenDestination.x , screenDestination.y};
            SDL_FRect rect = b -> getBoxCollider(position);
            
            if (!SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255))
            {
                SDL_Log("SDL_SetRenderDrawColor failed: %s\n", SDL_GetError());
            }
            
            SDL_RenderRect(renderer, &rect);
        }
        //SDL_Log("Render system working");
        
    }
}

