#pragma once

// ============================================================
//  core.h  —  Engine Class Declaration
//
//  ┌──────────────────────────────────────────────────────────┐
//  │  TASK OWNERSHIP MAP FOR THIS FILE                        │
//  │                                                          │
//  │  [Core Task 1] getInstance(), run()                      │
//  │                processInput(), updatePhysics()           │
//  │  [Core Task 2] render(), drawGrid(), drawHUD()  <- YOU   │
//  └──────────────────────────────────────────────────────────┘
//
//  The Engine is a Singleton — only ONE instance can ever exist.
//  All game systems access it via Engine::getInstance().
//
//  Frame lifecycle (enforced by Core Task 1):
//    processInput()   -> reads keyboard, delegates to player
//    updatePhysics()  -> calls PhysicsEngine, moves entities
//    render()         -> clears terminal, draws grid   <- YOU
// ============================================================

#include "Vec2.h"
#include <vector>
#include <string>

// Forward declarations (avoid circular includes)
class Entity;
class EntityManager;  // Gameplay Task 4
class PhysicsEngine;  // Gameplay Task 1

// ── Arena constants (shared by ALL teams — do not change) ─────────
constexpr int  GRID_WIDTH  = 20;
constexpr int  GRID_HEIGHT = 20;
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

    // ── [ CORE TASK 2 — YOUR RESPONSIBILITY ] ─────────────────────

    //  render()
    //  Top-level render call invoked by run() every frame.
    //  Clears the terminal, redraws the full 20x20 arena, then
    //  overlays every active entity by calling entity.display().
    void render();

    //  drawGrid()
    //  Builds the 20x20 char buffer populated with EMPTY_CELL,
    //  writes each active entity's display() char at its position,
    //  then prints the full buffer to stdout with border decoration.
    //
    //  Called by render() — do not call directly.
    void drawGrid(const std::vector<Entity*>& entities);

    //  drawHUD()
    //  Prints the status line below the grid:
    //    "Score: NNN  |  Ammo: NN  |  Player1: alive  |  Player2: alive"
    //  Data is pulled from EntityManager and Gameplay Task 3's shared stats.
    //
    //  Called by render() — do not call directly.
    void drawHUD();

    //  clearScreen()
    //  Issues the ANSI escape sequence to wipe the terminal before
    //  each frame so entities appear to move smoothly.
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