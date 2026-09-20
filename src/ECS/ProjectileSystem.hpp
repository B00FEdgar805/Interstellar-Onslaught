#ifndef ProjectileSystem_hpp
#define ProjectileSystem_hpp

#include "Components/Projectile.hpp"
#include "Registry.hpp"
#include "Types.hpp"
#include "CollisionSystem.hpp"
#include "DamageNumberSystem.hpp"


class ProjectileSystem
{
private:
    //float SPEED = 220.0f;
    Vector2D DIRECTION;
public:
    void projectileSystem(float delta_time);
    Entity createProjectile(Entity owner, const Vector2D& position, const Vector2D& direction, float speed, float damage, int type);
    void projectilesCollisons(std::vector<CollisionEvent>& collisons, DamageNumberSystem damage_num, float delta_time);
};
#endif /* ProjectileSystem_hpp */
