#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <vector>
#include "Entity.h" 
#include "Player.h"

// Forward declaration عشان الـ compiler يعرف إن فيه كلاس بالاسم ده
class PhysicsEngine; 

class GameWorld {
private:
    std::vector<Entity*> entities; // Part 1
    std::vector<Player*> players;  // Part 1
    PhysicsEngine* physicsEngine;  // Part 4: الربط بمحرك الفيزياء

public:
    // Constructor بياخد محرك الفيزياء
    GameWorld(PhysicsEngine* engine);

    void spawnEntity(Entity* entity);  // Part 2
    void removeEntity(Entity* entity); // Part 2
    void update(float deltaTime);      // Part 3
    void detectCollisions();           // Part 4: دالة كشف التصادم
};
#endif
