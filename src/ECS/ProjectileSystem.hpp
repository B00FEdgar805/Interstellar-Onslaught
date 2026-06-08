#ifndef ProjectileSystem_hpp
#define ProjectileSystem_hpp

#include "Components/Projectile.hpp"
#include "Registry.hpp"
#include "Types.hpp"
#include "CollisionSystem.hpp"


class ProjectileSystem
{
private:
    //float SPEED = 220.0f;
    float RATE_OF_FIRE = 100.0f;
    Vector2D DIRECTION;
public:
    void projectileSystem(Registry& registry, float delta_time);
    Entity createProjectile(Registry& registry, Entity owner, const Vector2D& position, const Vector2D& direction, float speed);
    void projectilesCollisons(Registry& registry, std::vector<CollisionEvent>& collisons);
};
#endif /* ProjectileSystem_hpp */
