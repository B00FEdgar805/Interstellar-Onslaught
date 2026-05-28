#ifndef BoxCollider_hpp
#define BoxCollider_hpp

#include "Component.hpp"
#include <cstdint>
#include <string>
#include <utility>

class BoxCollider final : public BaseComponent
{
public:
    Vector2D size{32.0f, 32.0f};
    Vector2D offset{0.0f, 0.0f};
    // If true collider detects overlap but does not physically push things apart.
    bool isTrigger = false;
    // If true object does not move during collision
    bool isStatic = false;
    // Optional name
    std::string tag;
    // Optional collision layers.
    std::uint32_t layer = 1;
    std::uint32_t collidesWith = 0xFFFFFFFFu;

    BoxCollider() = default;

    BoxCollider(float width, float height)
    {
        size.x = width;
        size.y = height;
    }

    BoxCollider(const Vector2D& size, const Vector2D& offset = Vector2D(0.0f, 0.0f), bool isTrigger = false, bool isStatic = false, std::string tag = "")
        : size(size),
          offset(offset),
          isTrigger(isTrigger),
          isStatic(isStatic),
          tag(std::move(tag))
    {}
};


#endif /* BoxCollider_hpp */
