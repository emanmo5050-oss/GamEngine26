#include <iostream>
#include "gameplay.h"
#include <algorithm>
#include <iostream>

// التنفيذ بتاع الـ Constructor
GameWorld::GameWorld(PhysicsEngine* engine) : physicsEngine(engine) {}

void GameWorld::spawnEntity(Entity* entity) {
    if (entity) {
        entities.push_back(entity);
    }
}

void GameWorld::removeEntity(Entity* entity) {
    // Part 2: مسح الـ entity من الـ vector
    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
}

void GameWorld::detectCollisions() {
    // Part 4: نداء محرك الفيزياء
    if (physicsEngine) {
        // بنفترض إن محرك الفيزياء فيه دالة اسمها checkCollisions
        // physicsEngine->checkCollisions(entities); 
        std::cout << "Checking collisions for all entities..." << std::endl;
    }
}

void GameWorld::update(float deltaTime) {
    // Part 3: تحديث كل الـ entities
    for (Entity* entity : entities) {
        if (entity) entity->update(deltaTime);
    }

    // Part 4: استدعاء كشف التصادم كل فريم
    detectCollisions();
}
using namespace std;
int main() {
    cout << "this is my team h3h3h3 " << endl;
    return 0;
}
