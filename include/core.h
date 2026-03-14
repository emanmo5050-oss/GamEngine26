#ifndef CORE_H
#define CORE_H
 // Here we will define all the functions you wrote in the core.cpp 
 //so that other tems can use them without referring to the long code.

#endif

// core.h
#pragma once
#include <iostream>
#include <atomic>
using namespace std;

class Engine {
private:
    // Singleton: private constructor & deleted copy
    Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    static Engine* instance;       // the one and only instance
    atomic<bool> running;          // thread-safe stop flag

public:
    static Engine* getInstance();  // global access point

    // Lifecycle
    void init();
    void run();
    void shutdown();

    // Loop steps (called inside run())
    void processInput();
    void updatePhysics();
    void updateGameLogic();
    void synchronizeNetwork();
    void render();

    bool isRunning() const;
};