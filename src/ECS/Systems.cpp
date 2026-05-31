#include "Systems.hpp"
#include "Components/PlayerControl.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"
#include "Components/Animation.hpp"


void Systems::playerInputSystem(Registry& registry)
{
    const bool* keys = SDL_GetKeyboardState(nullptr);

    for (auto& [entity, controlled] : registry.all<PlayerControl>())
    {
        (void)controlled;

        Velocity* velocity = registry.get<Velocity>(entity);

        if (!velocity)
        {
            continue;
        }

        constexpr float speed = 220.0f;

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
        
        velocity -> value.scale(speed);
        
        //SDL_Log("X %f", velocity -> value.x);
        //SDL_Log("Y %f", velocity -> value.y);

    }
}




void Systems::movementSystem(Registry& registry, float deltaTime)
{
    for (auto& [entity, velocity] : registry.all<Velocity>())
    {
        Transform* transform = registry.get<Transform>(entity);

        if (!transform)
        {
            continue;
        }
        
        transform -> position += velocity.value.scale(deltaTime);
        
    }
}

void Systems::renderSystem(Registry& registry, SDL_Renderer* renderer, const Camera2D& camera)  // Renders every entity that has a sprite
{
    for (auto& [entity, sprite] : registry.all<Sprite>())
    {
        Transform* transform = registry.get<Transform>(entity);
        Animation* animation = registry.get<Animation>(entity);
        PlayerControl* controlled = registry.get<PlayerControl>(entity);
        
        
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
        
        
        
        if (animation)
        {
            sprite.Animate(SDL_GetTicks(), animation -> speed, animation -> frames);
            //SDL_Log("Working");
        }
        
        if(controlled)
        {
            sprite.draw(angle);
        }
        else
        {
            sprite.draw();
        }
        
        //SDL_Log("Render system working");
        
    }
}
