#include <iostream>
#include <algorithm>
#include "gameplay.h"

using namespace std;

// Part 1 - Amy
// Constructor: initializes the GameWorld with a PhysicsEngine
GameWorld::GameWorld(PhysicsEngine* engine) : physicsEngine(engine) {}

// Part 1 - Amy
// Destructor: cleans up all entities from memory to prevent memory leaks
GameWorld::~GameWorld() {
    for (auto entity : entities) {
        delete entity;
    }
    entities.clear();
    players.clear();
}

// Part 2 - pending
// spawnEntity: adds a new entity to the game world
// removeEntity: removes an entity from the game world

// Part 3 - pending
// update: updates all entities every frame

// Part 4 - Abnob
// detectCollisions: checks for collisions using PhysicsEngine every frame
void GameWorld::detectCollisions() {
    if (physicsEngine) {
        cout << "Checking collisions for all entities..." << endl;
    }
}

// Part 4 - Abnob
// update: calls detectCollisions every frame
void GameWorld::update(float deltaTime) {
    for (Entity* entity : entities) {
        if (entity) entity->update(deltaTime);
    }
    detectCollisions();
}