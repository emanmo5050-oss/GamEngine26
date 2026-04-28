
#pragma once

#include <vector>
#include <memory>

class Entity;

class PhysicsEngine {
public:
    void init();
    void shutdown();

    void updatePositions(std::vector<std::unique_ptr<Entity>>& entities);
    void detectAndResolveCollisions(std::vector<std::unique_ptr<Entity>>& entities);

    // Legacy stubs — keep so core.cpp still compiles
    void detectCollisions() {}
    void resolveCollision() {}
    void updatePositions()  {}
};

//Reason: EntityManager.cpp calls updatePositions(entities) and detectAndResolveCollisions(entities) with the entity list as an
//argument. Without this update the project won't compile.
//Let me know if you have any questions!
 
/* 

1'st version of the PhysicsEngine class developed by Yassin Athula

PhysicsEngine.h

#pragma once

class PhysicsEngine {
public:
    void init();
    void updatePositions();
    void detectCollisions();
    void resolveCollision();
    void shutdown();
};
*/
