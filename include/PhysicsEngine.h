// PhysicsEngine.h
#pragma once

class PhysicsEngine {
public:
    void init();
    void updatePositions();
    void detectCollisions();
    void resolveCollision();
    void shutdown();
};