// ============================================================
//  core.cpp  —  Engine Implementation
//
//  ┌──────────────────────────────────────────────────────────┐
//  │  TASK OWNERSHIP MAP FOR THIS FILE                        │
//  │                                                          │
//  │  [DONE - Core Task 2] render()       <- YOUR WORK        │
//  │  [DONE - Core Task 2] drawGrid()     <- YOUR WORK        │
//  │  [DONE - Core Task 2] drawHUD()      <- YOUR WORK        │
//  │  [DONE - Core Task 2] clearScreen()  <- YOUR WORK        │
//  │  [TODO - Core Task 1] getInstance()  <- teammate          │
//  │  [TODO - Core Task 1] run()          <- teammate          │
//  │  [TODO - Core Task 1] processInput() <- teammate          │
//  │  [TODO - Core Task 1] updatePhysics()<- teammate          │
//  └──────────────────────────────────────────────────────────┘
//
//  Dependencies injected at runtime:
//    EntityManager (Gameplay Task 4) -> provides entity list
//    PhysicsEngine (Gameplay Task 1) -> called in updatePhysics()
//    Gameplay Task 3                 -> provides score & ammo
//    Network Task 2                  -> calls serialize after render
// ============================================================

#include "core.h"
#include "Entity.h"

// ── These headers will be provided by Gameplay Task 4 ─────────────
// #include "EntityManager.h"
// #include "PhysicsEngine.h"

#include <iostream>
#include <vector>
#include <string>

// ── Stub EntityManager so this file compiles standalone ───────────
//  Gameplay Task 4 will replace this with the real implementation.
class EntityManager {
public:
    std::vector<Entity*> getActiveEntities() { return {}; } // stub
    int getScore()  { return 0; }  // stub — Gameplay Task 3 fills this
    int getAmmo()   { return 0; }  // stub — Gameplay Task 3 fills this
};

// ── Stub PhysicsEngine so this file compiles standalone ───────────
//  Gameplay Task 1 (Eman) replaces this with the real class.
class PhysicsEngine {
public:
    void detectCollisions(std::vector<Entity*>&) {} // stub
    void resolveCollision(Entity*, Entity*)      {} // stub
};


// ── Static Singleton instance ──────────────────────────────────────
Engine* Engine::instance = nullptr;


// ================================================================
//  [ CORE TASK 2 — YOUR IMPLEMENTATION ]
//  clearScreen()
//
//  Wipes the terminal using the standard ANSI escape sequence
//  "\033[2J\033[H" which:
//    \033[2J  -> erase entire display
//    \033[H   -> move cursor to top-left (row 0, col 0)
//
//  This must be the FIRST thing called inside render() every frame
//  so entities appear to move rather than stack up.
//
//  Note for Network team: this only clears the local terminal;
//  network clients receive state via EntityManager::serializeAll().
// ================================================================
void Engine::clearScreen() {
    std::cout << "\033[2J\033[H" << std::flush;
}


// ================================================================
//  [ CORE TASK 2 — YOUR IMPLEMENTATION ]
//  drawGrid()
//
//  Renders the 20x20 arena to stdout every game tick.
//
//  Algorithm:
//   1. Allocate a 2D char buffer [GRID_HEIGHT][GRID_WIDTH] filled
//      with EMPTY_CELL ('.').
//   2. Iterate over `entities`. For each active entity, write
//      entity.display() at buffer[entity.y][entity.x].
//   3. Print the top border.
//   4. For each row, print '|' + the row chars + '|' + newline.
//   5. Print the bottom border.
//
//  How entity symbols map on-screen (20x20 example, y=0 is top):
//
//    ----------------------
//    |..........V.........|   <- row 0  (ceiling) Top Player = 'V'
//    |....................|
//    |....................|
//    |....................|
//    |......X.............|   <- Insect 'X' scrolling left
//    |....................|
//    |....................|
//    |.........M..........|   <- row 19 (floor)  Bottom Player = 'M'
//    ----------------------
//
//  Collision priority: if two entities share a cell, the last one
//  written wins. Physics (Gameplay Task 1) prevents real overlaps.
// ================================================================
void Engine::drawGrid(const std::vector<Entity*>& entities) {

    // ── Step 1: Allocate and fill buffer with empty cells ──────────
    char buffer[GRID_HEIGHT][GRID_WIDTH];
    for (int row = 0; row < GRID_HEIGHT; ++row)
        for (int col = 0; col < GRID_WIDTH; ++col)
            buffer[row][col] = EMPTY_CELL;

    // ── Step 2: Stamp each active entity onto the buffer ───────────
    for (const Entity* entity : entities) {
        if (!entity->isActive()) continue;          // Skip dead entities

        int x = entity->getPosition().x;
        int y = entity->getPosition().y;

        // Guard: clampPosition() (Gameplay Task 2) should prevent this,
        // but we double-check so a bug there can't crash rendering.
        if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT)
            continue;

        buffer[y][x] = entity->display();           // Core Task 2: display()
    }

    // ── Step 3: Print top border ───────────────────────────────────
    //  Format: "+" followed by GRID_WIDTH dashes, then "+"
    std::cout << '+';
    for (int col = 0; col < GRID_WIDTH; ++col)
        std::cout << BORDER_H;
    std::cout << "+\n";

    // ── Step 4: Print each row with side borders ───────────────────
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        std::cout << BORDER_V;                       // Left wall
        for (int col = 0; col < GRID_WIDTH; ++col)
            std::cout << buffer[row][col];
        std::cout << BORDER_V << '\n';               // Right wall + newline
    }

    // ── Step 5: Print bottom border ───────────────────────────────
    std::cout << '+';
    for (int col = 0; col < GRID_WIDTH; ++col)
        std::cout << BORDER_H;
    std::cout << "+\n";
}


// ================================================================
//  [ CORE TASK 2 — YOUR IMPLEMENTATION ]
//  drawHUD()
//
//  Prints the status bar below the arena grid. Reads live data
//  from EntityManager (Gameplay Task 3 writes Score & Ammo).
//
//  Output format (one line):
//    Score: 000  |  Ammo: 10  |  P1: alive  |  P2: alive
//
//  Integration points:
//    - score / ammo come from Gameplay Task 3 via Engine::getScore()
//      and Engine::getAmmo() (set each tick by gameplay.cpp)
//    - P1/P2 alive status: query EntityManager for TOP_PLAYER and
//      BOTTOM_PLAYER entities' isActive() flag.
// ================================================================
void Engine::drawHUD() {
    // ── Score and ammo (populated by Gameplay Task 3) ─────────────
    int currentScore = getScore();
    int currentAmmo  = getAmmo();

    // ── Player alive status (provided by Gameplay Task 4 manager) ─
    //  Stub: When EntityManager is integrated, query it like:
    //    bool p1Alive = entityManager->getTopPlayer()->isActive();
    //    bool p2Alive = entityManager->getBottomPlayer()->isActive();
    bool p1Alive = true;  // TODO: replace with entityManager query
    bool p2Alive = true;  // TODO: replace with entityManager query

    std::cout << "\nScore: " << currentScore
              << "  |  Ammo: "  << currentAmmo
              << "  |  P1 (Triangle): " << (p1Alive ? "alive" : "DEAD")
              << "  |  P2 (Ground):   " << (p2Alive ? "alive" : "DEAD")
              << "\n\n";

    // ── Controls reminder (shown every frame for usability) ────────
    std::cout << "P1 Controls: [A] Left  [D] Right  [SPACE] Shoot\n";
    std::cout << "P2 Controls: [<] Left  [>] Right  [SPACE] Jump\n";
}


// ================================================================
//  [ CORE TASK 2 — YOUR IMPLEMENTATION ]
//  render()
//
//  The full render pipeline called by run() every game tick.
//  Order is critical:
//   1. clearScreen()   — wipe previous frame from terminal
//   2. Get entity list — from EntityManager (Gameplay Task 4)
//   3. drawGrid()      — stamp entities onto 20x20 buffer & print
//   4. drawHUD()       — print score, ammo, player status line
//
//  Network Integration:
//   After render(), Core Task 1's run() will call the Network
//   Team's serializeAll() to broadcast updated state. The data
//   is already up-to-date because physics ran before render().
//
//  Frame order (enforced by Core Task 1's run()):
//    processInput() -> updatePhysics() -> render()   <- you are here
// ================================================================
void Engine::render() {

    // ── 1. Clear the terminal for a clean frame ────────────────────
    clearScreen();

    // ── 2. Fetch all active entities from EntityManager ───────────
    //  When Gameplay Task 4 delivers EntityManager, replace stub:
    //    std::vector<Entity*> entities = entityManager->getActiveEntities();
    std::vector<Entity*> entities = {};  // TODO: replace stub with real call

    // ── 3. Draw the 20x20 arena grid ──────────────────────────────
    drawGrid(entities);

    // ── 4. Draw the HUD status bar below the grid ─────────────────
    drawHUD();

    // ── 5. Flush stdout to ensure the frame appears immediately ───
    std::cout << std::flush;
}


// ================================================================
//  [ CORE TASK 1 — teammate implements this ]
//  getInstance()
//
//  Singleton accessor. Returns the one Engine instance.
//  Creates it on first call (lazy initialisation).
//
//  Required pattern:
//    private constructor + static instance pointer (see core.h)
// ================================================================
Engine& Engine::getInstance() {
    // TODO: Core Task 1 (teammate) ──────────────────────────────────
    //
    // Suggested sketch:
    //   if (instance == nullptr)
    //       instance = new Engine();
    //   return *instance;
    // ────────────────────────────────────────────────────────────────
    if (instance == nullptr)
        instance = new Engine();
    return *instance;
}


// ================================================================
//  [ CORE TASK 1 — teammate implements this ]
//  Engine() Constructor  (private — Singleton)
//
//  Initialise all subsystems. Order matters:
//   1. EntityManager (Gameplay Task 4)
//   2. PhysicsEngine (Gameplay Task 1)
//   3. running = true, score = 0, ammo = 0
// ================================================================
Engine::Engine()
    : running(true)
    , score(0)
    , ammo(0)
    , entityManager(new EntityManager())   // stub — Gameplay Task 4 replaces
    , physicsEngine(new PhysicsEngine())   // stub — Gameplay Task 1 replaces
{}

Engine::~Engine() {
    delete entityManager;
    delete physicsEngine;
    instance = nullptr;
}


// ================================================================
//  [ CORE TASK 1 — teammate implements this ]
//  run()
//
//  The master game loop. MUST execute in this exact order:
//    1. processInput()   — reads keyboard input
//    2. updatePhysics()  — moves entities, detects collisions
//    3. render()         — draws frame          <- your function
//    4. (Network) serializeAll() — broadcast state (Network Task 2)
//
//  Suggested loop style (fixed-tick, not frame-rate dependent):
//    while (running) {
//        processInput();
//        updatePhysics();
//        render();
//        // network broadcast here
//        sleep_for(TICK_DURATION);
//    }
// ================================================================
void Engine::run() {
    // TODO: Core Task 1 (teammate) ──────────────────────────────────
    while (running) {
        processInput();
        updatePhysics();
        render();
        // Network Team: call serializeAll() here and broadcast
    }
}


// ================================================================
//  [ CORE TASK 1 — teammate implements this ]
//  processInput()
//
//  Read raw keyboard input (non-blocking) and dispatch:
//    P1 (Top Player):    'a'/'d' -> move left/right
//                        'f'/'SPACE' -> shoot projectile
//    P2 (Bottom Player): arrow keys or 'j'/'l' -> move
//                        'SPACE' -> jump (impulse via Gameplay Task 2)
//    Global:             ESC / 'q' -> stop()
//
//  Network integration: in multiplayer mode, input from the remote
//  client arrives as a packet "PlayerID:Action:X:Y" (Network Task 1)
//  and should also be processed here or delegated.
// ================================================================
void Engine::processInput() {
    // TODO: Core Task 1 (teammate) ──────────────────────────────────
}


// ================================================================
//  [ CORE TASK 1 — teammate implements this ]
//  updatePhysics()
//
//  Drive the physics tick for one frame:
//   1. Move all entities one step (calls entity.move())
//   2. physicsEngine->detectCollisions(entities)
//   3. physicsEngine->resolveCollision() for each pair
//   4. EntityManager cleans up inactive entities (Gameplay Task 4)
//   5. EntityFactory spawns new insects if wave counter says so
//      (Gameplay Task 4)
// ================================================================
void Engine::updatePhysics() {
    // TODO: Core Task 1 (teammate) ──────────────────────────────────
}