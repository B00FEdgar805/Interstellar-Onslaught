#ifndef EnemyAI_hpp
#define EnemyAI_hpp

#include "Registry.hpp"
#include "CollisionSystem.hpp"
#include "Types.hpp"

class EnemyAi
{
private:
    Entity PLAYER;
    float TIMMER_ACCUMELATOR = 0.0f;
    float SPAWN_TIME = 5.0f;
    float PLAYER_DAMAGE_COOLDOWN_TIMER = 0.0f;
    float PLAYER_DAMAGE_COOLDOWN = 0.5f;
    float SPEED = 100.0f;
    
public:
    EnemyAi(Entity player)
    : PLAYER(player)
    {}
    
    void enemyAISystem();
    void createEnemy(const Vector2D& spawn, int type, float delta_time);
    void enemyCollisions(std::vector<CollisionEvent>& collisions, float delta);
    void enemySpawnSystem(float delta);
};
#endif /* EnemyAI_hpp */

