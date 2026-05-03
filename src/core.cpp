#include "core.h"
#include "Entity.h"
#include "EntityManager.h"
#include "PhysicsEngine.h"
#include "EntityFactory.h"
#include "Player.h"
#include "Enemy.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

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
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD topLeft = {0, 0};
    SetConsoleCursorPosition(hConsole, topLeft);
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
    std::cout << "\nhhhh bro it's too easy!"
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
    if (_kbhit()) {
        char key = static_cast<char>(toupper(_getch()));

        Player* top = nullptr;
        Player* bottom = nullptr;
        for (auto& e : entityManager->getEntities()) {
            if (auto* p = dynamic_cast<Player*>(e.get())) {
                if (p->isTop()) top = p;
                else bottom = p;
            }
        }

        switch (key) {
            case 'A': if (top) top->move({-1, 0}); break;
            case 'D': if (top) top->move({1, 0});  break;
            case 'S':
                if (top) {
                    std::unique_ptr<Entity> proj = top->shoot();
                    if (proj) entityManager->addEntity(std::move(proj));
                }
                break;
            case 'H': if (bottom) bottom->move({-1, 0}); break;
            case 'L': if (bottom) bottom->move({1, 0});  break;
            case ' ':
                if (bottom)
                    bottom->setVelocity({0, -5});
                break;
            case 'Q': case 27: running = false; break;
        }
    }
}

void Engine::updatePhysics() {
    for (auto& e : entityManager->getEntities()) {
        if (e && e->isActive())
            e->move({0, 0});
    }
    physicsEngine->detectAndResolveCollisions(entityManager->getEntities());

    // Check if any player is dead — stop the game
    for (auto& e : entityManager->getEntities()) {
        if (auto* p = dynamic_cast<Player*>(e.get())) {
            if (!p->isActive()) {
                running = false;
            }
        }
    }

    // Respawn enemy if no active enemy exists
    bool hasEnemy = false;
    for (auto& e : entityManager->getEntities()) {
        if (e && e->isActive() && e->getType() == EntityType::INSECT)
            hasEnemy = true;
    }
    if (!hasEnemy) {
        score++;
        entityManager->addEntity(EntityFactory::createEntity("Enemy", rand() % 1000, {rand() % 38 + 1, 1}));
    }
}

void Engine::run() {
    srand(time(nullptr));

    auto p1 = EntityFactory::createEntity("Player", 1, {20, 0}, "Top");


    if (p1) {
        static_cast<Player*>(p1.get())->setAsTopPlayer(true);
        entityManager->addEntity(std::move(p1));
    }

    auto p2 = EntityFactory::createEntity("Player", 2, {20, 19}, "Bottom");
    if (p2) {
        static_cast<Player*>(p2.get())->setAsTopPlayer(false);
        entityManager->addEntity(std::move(p2));
    }

    entityManager->addEntity(EntityFactory::createEntity("Enemy", 3, {rand() % 38 + 1, 1}));
    while (running) {
        processInput();
        updatePhysics();
        render();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }

    // Game Over screen
    system("cls");
    std::cout << "\n\n";
    std::cout << "===============================================\n";
    std::cout << "                Damn you lost!                 \n";
    std::cout << "           really dude it's too easy           \n";
    std::cout << "===============================================\n";
    std::cout << "\nPress any key to exit...\n";
    _getch();
}