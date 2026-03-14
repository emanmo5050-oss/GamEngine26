#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <vector>
#include "Entity.h" // افترضنا إن فيه كلاس اسمه Entity موجود فعلاً
#include "Player.h"

class GameWorld {
private:
    std::vector<Entity*> entities; // Part 1
    std::vector<Player*> players;  // Part 1

public:
    void spawnEntity(Entity* entity); // Part 2
    void removeEntity(Entity* entity); // Part 2
    void update(float deltaTime);      // Part 3
};

#endif
