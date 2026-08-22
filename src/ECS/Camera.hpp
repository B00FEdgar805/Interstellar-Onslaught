#ifndef Camera_hpp
#define Camera_hpp

#include "../Math.hpp"

#include <SDL3/SDL.h>

class Camera2D
{
public:
    Vector2D POSITION{0.0f, 0.0f};  // Used to keep position of camera

    float VIEWPORT_WIDTH = 800.0f;
    float VIEWPORT_HEIGHT = 600.0f;

public:
    Camera2D() = default;

    Camera2D(float viewportWidth, float viewportHeight)
        : VIEWPORT_WIDTH(viewportWidth),
          VIEWPORT_HEIGHT(viewportHeight)
    {}

    SDL_FRect getViewRect() const
    {
        return SDL_FRect{POSITION.x, POSITION.y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT};
    }

    Vector2D worldToScreen(const Vector2D& worldPosition) const
    {
        return Vector2D(worldPosition.x - POSITION.x, worldPosition.y - POSITION.y);
    }

    SDL_FRect worldToScreenRect(const SDL_FRect& worldRect) const
    {
        return SDL_FRect{ worldRect.x - POSITION.x, worldRect.y - POSITION.y, worldRect.w, worldRect.h};
    }

    Vector2D screenToWorld(const Vector2D& screenPosition) const
    {
        // Inverse of worldToScreen: screen = world - POSITION  ->  world = screen + POSITION
        return Vector2D(screenPosition.x + POSITION.x, screenPosition.y + POSITION.y);
    }

    SDL_FRect screenToWorldRect(const SDL_FRect& screenRect) const
    {
        return SDL_FRect{ screenRect.x + POSITION.x, screenRect.y + POSITION.y, screenRect.w, screenRect.h };
    }
    
    void centerOn(const Vector2D& targetPosition)
    {
        POSITION.x = targetPosition.x - VIEWPORT_WIDTH * 0.5f;
        POSITION.y = targetPosition.y - VIEWPORT_HEIGHT * 0.5f;
    }

    void follow(const Vector2D& targetPosition, float smoothing, float deltaTime)
    {
        Vector2D desiredPosition( targetPosition.x - VIEWPORT_WIDTH * 0.5f, targetPosition.y - VIEWPORT_HEIGHT * 0.5f);

        POSITION.x += (desiredPosition.x - POSITION.x) * smoothing * deltaTime;
        POSITION.y += (desiredPosition.y - POSITION.y) * smoothing * deltaTime;
    }

    void clampToWorld(float worldWidth, float worldHeight)
    {
        if (POSITION.x < 0.0f)
        {
            POSITION.x = 0.0f;
        }

        if (POSITION.y < 0.0f)
        {
            POSITION.y = 0.0f;
        }

        float maxX = worldWidth - VIEWPORT_WIDTH;
        float maxY = worldHeight - VIEWPORT_HEIGHT;

        if (POSITION.x > maxX)
        {
            POSITION.x = maxX;
        }

        if (POSITION.y > maxY)
        {
            POSITION.y = maxY;
        }

        // If the world is smaller than the screen, keep camera at 0.
        if (worldWidth <= VIEWPORT_WIDTH)
        {
            POSITION.x = 0.0f;
        }

        if (worldHeight <= VIEWPORT_HEIGHT)
        {
            POSITION.y = 0.0f;
        }
    }
};
#endif /* Camera_hpp */
