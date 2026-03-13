// core.cpp
#include "core.h"
#include <iostream>
using namespace std;

Engine* Engine::instance = nullptr;   // initialize static member

Engine::Engine() : running(false) {}  // private constructor

Engine* Engine::getInstance() {
    if (instance == nullptr)
        instance = new Engine();
    return instance;
}

void Engine::init() {
    cout << "[Engine] Initializing subsystems..." << endl;
    running = true;

    // TODO Task 4: physicsEngine.init();
    // TODO Network team: networkManager.init();
    // TODO Gameplay team: gameWorld.init();

    cout << "[Engine] Engine ready." << endl;
}

void Engine::shutdown() {
    running = false;
    cout << "[Engine] Shutting down..." << endl;

    // TODO: cleanup subsystems in reverse order
}

bool Engine::isRunning() const {
    return running;
}

void Engine::run() {
    cout << "[Engine] Starting game loop..." << endl;

    while (running) {
        processInput();
        updatePhysics();
        updateGameLogic();
        synchronizeNetwork();
        render();
    }

    shutdown();
}

void Engine::processInput() {
    // TODO: forward to InputSystem / Network (player commands)
}

void Engine::updatePhysics() {
    // TODO Task 4: physicsEngine.updatePositions();
    //              physicsEngine.detectCollisions();
}

void Engine::updateGameLogic() {
    // TODO Gameplay Task 1: gameWorld.update();
}

void Engine::synchronizeNetwork() {
    // TODO Network team: networkManager.broadcastState();
}

void Engine::render() {
    // TODO Gameplay Task 3: renderer.render();
}

int main() {
    Engine* engine = Engine::getInstance();
    engine->init();
    engine->run();   // blocks until running = false
    return 0;
}