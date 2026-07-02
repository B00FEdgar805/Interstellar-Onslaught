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


    
public:
    EnemyAi(Entity player)
    : PLAYER(player)
    {}
    
    void enemyAISystem(Registry& registry);
    void createEnemy(Registry& registry, const Vector2D& spawn, float speed, float delta_time);
    void enemyCollisions(Registry& registry, std::vector<CollisionEvent>& collisions);
    void enemySpawnSystem(Registry& registry, float delta);
};
#endif /* EnemyAI_hpp */
