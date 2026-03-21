// core.h
// Task 1: Engine Class (Improved Version)
#pragma once
#include <iostream>
#include <atomic>
#include "EntityManager.h"
#include "PhysicsEngine.h"

class Engine {
private:
    // Singleton Pattern
    Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    static Engine* instance;

    std::atomic<bool> running{false};

    // Subsystems
    EntityManager entityManager;
    PhysicsEngine physicsEngine;

public:
    static Engine* getInstance();

    // Lifecycle
    void init();
    void run();
    void shutdown();

    // Game Loop
    void processInput();
    void updatePhysics();
    void updateGameLogic();
    void synchronizeNetwork();
    void render();

    bool isRunning() const;

    // Getters
    EntityManager& getEntityManager() { return entityManager; }
    PhysicsEngine& getPhysicsEngine() { return physicsEngine; }
};