#include "Systems.hpp"
#include "Components/PlayerControl.hpp"
#include "Components/Velocity.hpp"
#include "Components/Sprite.hpp"
#include "Components/Transform.hpp"


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

        velocity -> x = 0.0f;
        velocity -> y = 0.0f;

        if (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT])
        {
            velocity -> x -= speed;
        }

        if (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT])
        {
            velocity -> x += speed;
        }

        if (keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_UP])
        {
            velocity -> y -= speed;
        }

        if (keys[SDL_SCANCODE_S] || keys[SDL_SCANCODE_DOWN])
        {
            velocity -> y += speed;
        }
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

        transform -> x += velocity.x * deltaTime;
        transform -> y += velocity.y * deltaTime;
    }
}

void Systems::renderSystem(Registry& registry, SDL_Renderer* renderer)  // Renders every entity that has a sprite
{
    for (auto& [entity, sprite] : registry.all<Sprite>())
    {
        Transform* transform = registry.get<Transform>(entity);

        if (!transform)
        {
            continue;
        }

        sprite.x(transform -> x);
        sprite.y(transform -> y);
        //SDL_Log("%f", transform -> x);
        //SDL_Log("%f", transform -> y);

        
        sprite.draw();
        //SDL_Log("Render system working");
        
    }
}
