#include "core.h"
#include "Entity.h"
#include "EntityManager.h"
#include "PhysicsEngine.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <conio.h>

Engine* Engine::instance = nullptr;

Engine::Engine()
    : running(true)
    , score(0)
    , ammo(0)
    , entityManager(new EntityManager())
    , physicsEngine(new PhysicsEngine())
{}

Engine::~Engine() {
    delete entityManager;
    delete physicsEngine;
    instance = nullptr;
}

Engine& Engine::getInstance() {
    if (instance == nullptr)
        instance = new Engine();
    return *instance;
}

void Engine::clearScreen() {
    system("cls");  // Windows clear — more reliable than ANSI
}

void Engine::drawGrid(const std::vector<Entity*>& entities) {
    char buffer[GRID_HEIGHT][GRID_WIDTH];
    for (int row = 0; row < GRID_HEIGHT; ++row)
        for (int col = 0; col < GRID_WIDTH; ++col)
            buffer[row][col] = EMPTY_CELL;

    for (const Entity* entity : entities) {
        if (!entity->isActive()) continue;
        int x = entity->getPosition().x;
        int y = entity->getPosition().y;
        if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) continue;
        buffer[y][x] = entity->display();
    }

    std::cout << '+';
    for (int col = 0; col < GRID_WIDTH; ++col) std::cout << BORDER_H;
    std::cout << "+\n";

    for (int row = 0; row < GRID_HEIGHT; ++row) {
        std::cout << BORDER_V;
        for (int col = 0; col < GRID_WIDTH; ++col)
            std::cout << buffer[row][col];
        std::cout << BORDER_V << '\n';
    }

    std::cout << '+';
    for (int col = 0; col < GRID_WIDTH; ++col) std::cout << BORDER_H;
    std::cout << "+\n";
}

void Engine::drawHUD() {
    std::cout << "\nScore: " << score
              << "  |  Ammo: " << ammo
              << "  |  P1 (Triangle): alive"
              << "  |  P2 (Ground): alive\n\n";
    std::cout << "P1: [A] Left  [D] Right  [S] Shoot\n";
    std::cout << "P2: [H] Left  [L] Right  [SPACE] Jump  |  [Q] Quit\n";
}

void Engine::render() {
    clearScreen();
    std::vector<Entity*> entities;
    for (auto& e : entityManager->getEntities())
        if (e && e->isActive())
            entities.push_back(e.get());
    drawGrid(entities);
    drawHUD();
    std::cout << std::flush;
}

void Engine::processInput() {
    if (!_kbhit()) return;
    char key = static_cast<char>(toupper(_getch()));
    if (key == 'Q' || key == 27) running = false;
}

void Engine::updatePhysics() {
    // TODO: connect to real physics once entities are spawned
}

void Engine::run() {
    while (running) {
        processInput();
        updatePhysics();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}