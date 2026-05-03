#pragma once


#include "Vec2.h"
#include <vector>
#include <string>

// Forward declarations (avoid circular includes)
class Entity;
class EntityManager;  // Gameplay Task 4
class PhysicsEngine;  // Gameplay Task 1

// ── Arena constants (shared by ALL teams — do not change) ─────────
constexpr int GRID_WIDTH  = 40;
constexpr int GRID_HEIGHT = 20;
constexpr char EMPTY_CELL  = '.';   // Empty cell character
constexpr char BORDER_H    = '-';   // Horizontal border
constexpr char BORDER_V    = '|';   // Vertical border

// ── Engine ────────────────────────────────────────────────────────
class Engine {
public:
    // ── [ CORE TASK 1 — Singleton boilerplate ] ───────────────────
    static Engine& getInstance();

    // Deleted to prevent copies — Singleton contract
    Engine(const Engine&)            = delete;
    Engine& operator=(const Engine&) = delete;

    // ── [ CORE TASK 1 — Master loop ] ─────────────────────────────
    void run();
    void processInput();
    void updatePhysics();


    void render();

    void drawGrid(const std::vector<Entity*>& entities);

    
    void drawHUD();

    
    void clearScreen();

    // ── Accessors (shared utilities) ──────────────────────────────
    bool isRunning() const { return running; }
    void stop()            { running = false; }

    // Score & ammo — written by Gameplay Task 3, read by drawHUD()
    int  getScore()        const { return score; }
    int  getAmmo()         const { return ammo;  }
    void setScore(int s)         { score = s;    }
    void setAmmo(int a)          { ammo  = a;    }

private:
    // ── [ CORE TASK 1 — Private constructor (Singleton) ] ─────────
    Engine();
    ~Engine();

    // ── Internal state ────────────────────────────────────────────
    bool           running;
    int            score;
    int            ammo;

    // Owned subsystems — instantiated in constructor
    EntityManager* entityManager;   // Gameplay Task 4 fills this
    PhysicsEngine* physicsEngine;   // Gameplay Task 1 fills this

    // Singleton instance
    static Engine* instance;
};