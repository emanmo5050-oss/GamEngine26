// core.cpp
// Task 1: Engine Class Implementation (Improved Version)
#include "../include/core.h"
#include "EntityFactory.h"
#include <iostream>
#include <chrono>
#include <thread>       // ← add this

Engine* Engine::instance = nullptr;

Engine::Engine() = default;

Engine* Engine::getInstance() {
    if (instance == nullptr) {
        instance = new Engine();
    }
    return instance;
}

void Engine::init() {
    std::cout << "[Engine] Initializing subsystems...\n";

    running = true;

    // إنشاء الكيانات باستخدام Factory Pattern
    entityManager.addEntity(EntityFactory::createEntity("Player",     1, {5,  5}, "Yassin"));
    entityManager.addEntity(EntityFactory::createEntity("Enemy",      2, {12, 8}));
    entityManager.addEntity(EntityFactory::createEntity("PowerUp",    3, {15, 15}));
    entityManager.addEntity(EntityFactory::createEntity("Obstacle",   4, {8,  10}));
    entityManager.addEntity(EntityFactory::createEntity("Projectile", 5, {6,  6}));

    std::cout << "[Engine] " << entityManager.getCount() 
              << " entities initialized successfully.\n";
    std::cout << "[Engine] Engine initialized successfully.\n";
}

void Engine::shutdown() {
    running = false;
    entityManager.clear();
    std::cout << "[Engine] Engine shut down.\n";
}

bool Engine::isRunning() const {
    return running;
}

void Engine::run() {
    std::cout << "[Engine] Starting game loop...\n";

    while (running) {
        processInput();
        updatePhysics();
        updateGameLogic();
        synchronizeNetwork();
        render();

        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // \~10 FPS
    }

    shutdown();
}

void Engine::processInput() {
    // TODO: سيتم ربطه مع نظام الإدخال لاحقاً
}

void Engine::updatePhysics() {
    // يمكن إضافة منطق إضافي هنا في المستقبل
}

void Engine::updateGameLogic() {
    entityManager.updateAll(physicsEngine);
}

void Engine::synchronizeNetwork() {
    std::string state = entityManager.serializeAll();
    // TODO: إرسال state إلى الـ Clients (فريق Networking)
}

void Engine::render() {
    constexpr int WIDTH = 20;
    constexpr int HEIGHT = 20;

    std::vector<std::vector<char>> grid(HEIGHT, std::vector<char>(WIDTH, '.'));

    entityManager.renderAll(grid);

    system("cls");

    for (const auto& row : grid) {
        for (char c : row) std::cout << c << ' ';
        std::cout << '\n';
    }

    std::cout << "=== Multiplayer Arena Shooter ===\n";
    std::cout << "Entities: " << entityManager.getCount() << " | FPS: \~10\n";
}