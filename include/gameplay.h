#pragma once
#include <vector>
#include "Entity.h"
#include "Player.h"

using namespace std;

// Forward declaration
class PhysicsEngine;

// Part 1 - Amy
// GameWorld: main class that contains all entities and players
class GameWorld {
private:
    vector<Entity*> entities;
    vector<Player*> players;

    // Part 4 - Abnob
    PhysicsEngine* physicsEngine;

public:
    // Part 1 - Amy
    GameWorld(PhysicsEngine* engine);
    ~GameWorld();

    // Part 2 - pending

    // Part 3 - pending

    // Part 4 - Abnob
    void update(float deltaTime);
    void detectCollisions();
};