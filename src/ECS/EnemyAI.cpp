//
//  EnemyAI.cpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 6/5/26.
//

#include "EnemyAI.hpp"
#include "Components/Enemy.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Health.hpp"
#include "Components/Transform.hpp"



void EnemyAi::enemyAISystem(Registry &registry)
{
    Transform* playerPos = registry.get<Transform>(PLAYER);
    auto view = registry.all<Enemy>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Enemy& enemy = view.components[i];
        (void)enemy;
        Velocity* enemyDir = registry.get<Velocity>(entity);
        Transform* enemyPos = registry.get<Transform>(entity);

        if (!enemyDir)
        {
            continue;
        }

        if (!enemyPos)
        {
            continue;
        }
        auto temp = playerPos -> position;
        enemyDir -> direction = temp - enemyPos -> position;

        enemyDir -> value.zero();
        enemyDir -> value += enemyDir -> direction.normalize();

    }
}

void EnemyAi::createEnemy(Registry &registry, const Vector2D& position, float speed)
{
    Entity enemy = registry.create();
    registry.add(enemy, Enemy());
    registry.add(enemy, Transform(position));
    registry.add(enemy, Velocity(speed));
    registry.add(enemy, BoxCollider(Vector2D(32.0f, 32.0f), Vector2D(0.0f, 0.0f), false, false, "enemy"));
    registry.add(enemy, Sprite("Enemy"));
    registry.add(enemy, Health(5.0f, 10.0f));
}

void EnemyAi::enemyCollisions(Registry& registry, std::vector<CollisionEvent>& collisions)
{
    for (const CollisionEvent& collision : collisions)
    {
        BoxCollider* a = registry.get<BoxCollider>(collision.a);
        BoxCollider* b = registry.get<BoxCollider>(collision.b);

        if (a == nullptr || b == nullptr)
        {
            continue;
        }

        if (((a -> tag == "player" && b -> tag == "enemy") || (a -> tag == "enemy" && b -> tag == "player")))
        {
            Health* player_health = registry.get<Health>(PLAYER);
            //Enemy* enemy = registry.get<Enemy>();
            player_health -> takeDamage(10.0f);
            if (!player_health -> isAlive())
            {
                //SDL_Log("Dead");
                //registry.destroy(PLAYER);
                // add damage to box collider instead
            }
        }
    }
}

