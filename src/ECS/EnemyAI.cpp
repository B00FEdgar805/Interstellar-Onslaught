#include "EnemyAI.hpp"
#include "../Globals.hpp"
#include "Components/Enemy.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Health.hpp"
#include "Components/Transform.hpp"
#include "Components/Animation.hpp"



void EnemyAi::enemyAISystem()
{
    Transform* playerPos = GLOBALS::REGISTRY.get<Transform>(PLAYER);
    auto view = GLOBALS::REGISTRY.all<Enemy>();
    for (size_t i = 0; i < view.entities.size(); ++i)
    {
        Entity entity = view.entities[i];
        Enemy& enemy = view.components[i];
        (void)enemy;
        Velocity* enemyDir = GLOBALS::REGISTRY.get<Velocity>(entity);
        Transform* enemyPos = GLOBALS::REGISTRY.get<Transform>(entity);

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

void EnemyAi::createEnemy(const Vector2D& position, float speed, float delta_time)
{
    Entity enemy = GLOBALS::REGISTRY.create();
    GLOBALS::REGISTRY.add(enemy, Enemy(10.0f));
    GLOBALS::REGISTRY.add(enemy, Transform(position));
    GLOBALS::REGISTRY.add(enemy, Velocity(speed));
    GLOBALS::REGISTRY.add(enemy, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "enemy"));
    GLOBALS::REGISTRY.add(enemy, Sprite("Enemy", Vector2D(16.0f, 16.0f)));
    GLOBALS::REGISTRY.add(enemy, Animation(delta_time, 2, 150));
    GLOBALS::REGISTRY.add(enemy, Health(5.0f));
    
}

void EnemyAi::enemyCollisions(std::vector<CollisionEvent>& collisions)
{
    for (const CollisionEvent& collision : collisions)
    {
        BoxCollider* a = GLOBALS::REGISTRY.get<BoxCollider>(collision.a);
        BoxCollider* b = GLOBALS::REGISTRY.get<BoxCollider>(collision.b);

        if (a == nullptr || b == nullptr)
        {
            continue;
        }
    
        
        if ((a -> tag == "player" && b -> tag == "enemy"))
        {
            Health* player_health = GLOBALS::REGISTRY.get<Health>(collision.a);
            Enemy* enemy = GLOBALS::REGISTRY.get<Enemy>(collision.b);
            
            player_health -> takeDamage(enemy -> getDamage());
            
            if (!player_health -> isAlive())
            {
                //SDL_Log("Dead");
                //GLOBALS::REGISTRY.destroy(PLAYER);
                // add damage to box collider instead
            }
        }
        else if ((a -> tag == "enemy" && b -> tag == "player"))
        {
            Health* player_health = GLOBALS::REGISTRY.get<Health>(collision.b);
            Enemy* enemy = GLOBALS::REGISTRY.get<Enemy>(collision.a);

            player_health -> takeDamage(enemy -> getDamage());
            
            if (!player_health -> isAlive())
            {
               // SDL_Log("Dead");
            }
        }
    }
}

void EnemyAi::enemySpawnSystem(float delta)
{
    TIMMER_ACCUMELATOR += delta;
    if (TIMMER_ACCUMELATOR >= SPAWN_TIME)
    {
        // add a way to spawn eneimies left/righ and up/down the viewport
        createEnemy(Vector2D(10.0f, 10.0f), 100.0f, delta);
        SPAWN_TIME *= 0.95;
        TIMMER_ACCUMELATOR = 0.0f;
    }
}

