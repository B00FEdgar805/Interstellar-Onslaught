#include "EnemyAI.hpp"
#include "../Globals.hpp"
#include "Components/Enemy.hpp"
#include "Components/Velocity.hpp"
#include "Components/BoxCollider.hpp"
#include "Components/Sprite.hpp"
#include "Components/Health.hpp"
#include "Components/Transform.hpp"
#include "Components/Animation.hpp"

//static constexpr float PLAYER_DAMAGE_COOLDOWN = 0.5f;

//float EnemyAi::playerDamageCooldownTimer = 0.0f;

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
        if (!GLOBALS::FREEZE)
        {
            enemyDir -> value += enemyDir -> direction.normalize();
        }

    }
}

void EnemyAi::createEnemy(const Vector2D& position, int type, float delta_time)
{
    switch (type)
    {
        case 1: // Quick enemy
        {
            Entity enemy = GLOBALS::REGISTRY.create();
            GLOBALS::REGISTRY.add(enemy, Enemy(10.0f * SCALER));
            GLOBALS::REGISTRY.add(enemy, Transform(position));
            GLOBALS::REGISTRY.add(enemy, Velocity(SPEED * 1.5f));
            GLOBALS::REGISTRY.add(enemy, BoxCollider(Vector2D(16.0f, 16.0f), Vector2D(0.0f, 0.0f), false, false, "enemy"));
            GLOBALS::REGISTRY.add(enemy, Sprite("Enemy1", Vector2D(16.0f, 16.0f)));
            GLOBALS::REGISTRY.add(enemy, Animation(delta_time, 2, 150));
            GLOBALS::REGISTRY.add(enemy, Health(5.0f * SCALER));
            Sprite* s = GLOBALS::REGISTRY.get<Sprite>(enemy);
            s -> setRow(ROW);
        }
            break;
        case 2: // Balanced
        {
            Entity enemy = GLOBALS::REGISTRY.create();
            GLOBALS::REGISTRY.add(enemy, Enemy(20.0f * SCALER));
            GLOBALS::REGISTRY.add(enemy, Transform(position));
            GLOBALS::REGISTRY.add(enemy, Velocity(SPEED));
            GLOBALS::REGISTRY.add(enemy, BoxCollider(Vector2D(32.0f, 32.0f), Vector2D(0.0f, 0.0f), false, false, "enemy"));
            GLOBALS::REGISTRY.add(enemy, Sprite("Enemy2", Vector2D(32.0f, 32.0f)));
            GLOBALS::REGISTRY.add(enemy, Animation(delta_time, 2, 150));
            GLOBALS::REGISTRY.add(enemy, Health(30.0f * SCALER));
            Sprite* s = GLOBALS::REGISTRY.get<Sprite>(enemy);
            s -> setRow(ROW);
        }
            break;
        case 3: // Slow
        {
            Entity enemy = GLOBALS::REGISTRY.create();
            GLOBALS::REGISTRY.add(enemy, Enemy(40.0f * SCALER));
            GLOBALS::REGISTRY.add(enemy, Transform(position));
            GLOBALS::REGISTRY.add(enemy, Velocity(SPEED * 0.5f));
            GLOBALS::REGISTRY.add(enemy, BoxCollider(Vector2D(32.0f, 32.0f), Vector2D(0.0f, 0.0f), false, false, "enemy"));
            GLOBALS::REGISTRY.add(enemy, Sprite("Enemy3", Vector2D(32.0f, 32.0f)));
            GLOBALS::REGISTRY.add(enemy, Animation(delta_time, 2, 150));
            GLOBALS::REGISTRY.add(enemy, Health(100.0f * SCALER));
            Sprite* s = GLOBALS::REGISTRY.get<Sprite>(enemy);
            s -> setRow(ROW);
        }
            break;
        default:
            break;
    }
    
}

void EnemyAi::enemyCollisions(std::vector<CollisionEvent>& collisions, float delta)
{
    PLAYER_DAMAGE_COOLDOWN_TIMER -= delta;
    if (PLAYER_DAMAGE_COOLDOWN_TIMER < 0.0f) PLAYER_DAMAGE_COOLDOWN_TIMER = 0.0f;

    if (GLOBALS::INVINCIBLE_CONSTANT)
    {
        return;
    }
    
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

            if (PLAYER_DAMAGE_COOLDOWN_TIMER <= 0.0f && !GLOBALS::INVINCIBLE)
            {
                Enemy* enemy = GLOBALS::REGISTRY.get<Enemy>(collision.b);
                player_health -> takeDamage(enemy -> getDamage());
                PLAYER_DAMAGE_COOLDOWN_TIMER = PLAYER_DAMAGE_COOLDOWN;
                //SDL_Log("Hit");
                //SDL_Log("%f", player_health -> getHealth());

            }
            
            
            if (!player_health -> isAlive())
            {
                //SDL_Log("Dead");
                player_health -> setHealth(0.0f);
                //GLOBALS::REGISTRY.destroy(PLAYER);
                // add damage to box collider instead
            }
           
            GLOBALS::INVINCIBLE = false;
        }
        else if ((a -> tag == "enemy" && b -> tag == "player"))
        {
            Health* player_health = GLOBALS::REGISTRY.get<Health>(collision.b);

            if (PLAYER_DAMAGE_COOLDOWN_TIMER <= 0.0f && !GLOBALS::INVINCIBLE)
            {
                Enemy* enemy = GLOBALS::REGISTRY.get<Enemy>(collision.a);
                player_health -> takeDamage(enemy -> getDamage());
                PLAYER_DAMAGE_COOLDOWN_TIMER = PLAYER_DAMAGE_COOLDOWN;
                //SDL_Log("Hit");
                //SDL_Log("%f", player_health -> getHealth());
            }
            
            
            if (!player_health -> isAlive())
            {
                player_health -> setHealth(0.0f);
               // SDL_Log("Dead");
            }
            
            GLOBALS::INVINCIBLE = false;
        }
        
       

    }
}

void EnemyAi::enemySpawnSystem(float delta)
{
    auto view = GLOBALS::REGISTRY.all<Enemy>();
    if (view.entities.size() > 150)
    {
        return;
    }
    
    TIMMER_ACCUMELATOR += delta;
    if (TIMMER_ACCUMELATOR >= SPAWN_TIME)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distr(1, 4);
        int num = distr(gen);
        float x = -32.0f;
        float y = -32.0f;
        switch (num)
        {
            case 1:
                // keep x and y -32
                break;
            case 2:
                x = GLOBALS::SCREEN_WIDTH + 32.0f;
                break;
            case 3:
                y = GLOBALS::SCREEN_HEIGHT + 32.0f;
                break;
            case 4:
            {
                x = GLOBALS::SCREEN_WIDTH + 32.0f;
                y = GLOBALS::SCREEN_HEIGHT + 32.0f;
            }
                break;
            default:
                break;
        }
        
        
        for(int i = 0; i < MAX_FAST; i++)
        {
            createEnemy(Vector2D(x,y), 1, delta);
        }
        
        for (int i = 0; i < MAX_NORMAL; i++)
        {
            createEnemy(Vector2D(x,y), 2, delta);
        }
        
        for (int i = 0; i < MAX_SLOW; i++)
        {
            createEnemy(Vector2D(x,y), 3, delta);
        }

        SPAWN_TIME -= 0.25f;
        if (SPAWN_TIME <= 3.0f)
        {
            SPAWN_TIME = 3.0f;
        }
        
        TIMMER_ACCUMELATOR = 0.0f;
        WAVE++;
        if (WAVE < 60)
        {
            if(WAVE >= 20 && WAVE % 2 == 0)
            {
                ROW = 2;
                MAX_FAST++;
                MAX_NORMAL++;
                MAX_SLOW++;
            }
            if(WAVE >= 40 && WAVE % 2 == 0)
            {
                MAX_NORMAL++;
                MAX_SLOW++;
            }
        }
        else
        {
            ROW = 3;
            // Here would be the code for making enemies hardeer
            SCALER *= 1.1f;
        }
        
        //SDL_Log("%f", SPAWN_TIME);
        
    }
}

