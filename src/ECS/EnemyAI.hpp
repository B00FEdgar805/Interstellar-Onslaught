//
//  EnemyAI.hpp
//  GameTestSDL3
//
//  Created by Edgar Alamillo on 6/5/26.
//

#ifndef EnemyAI_hpp
#define EnemyAI_hpp

#include "Registry.hpp"
#include "CollisionSystem.hpp"
#include "Types.hpp"

class EnemyAi
{
private:
    Entity PLAYER;
public:
    EnemyAi(Entity player)
    : PLAYER(player)
    {}
    
    void enemyAISystem(Registry& registry);
    void createEnemy(Registry& registry, const Vector2D& spawn, float speed);
    void enemyCollisions(Registry& registry, std::vector<CollisionEvent>& collisions);
};
#endif /* EnemyAI_hpp */
