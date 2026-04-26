// ============================================================
//  Entity.cpp  —  Entity Base Implementation
//
//  ┌──────────────────────────────────────────────────────────┐
//  │  TASK OWNERSHIP MAP FOR THIS FILE                        │
//  │                                                          │
//  │  [DONE - Core Task 2]     display()       <- YOUR WORK   │
//  │  [DONE - Core Task 2]     serialize()     <- YOUR WORK   │
//  │  [TODO - Gameplay Task 2] move()          <- Eman friend  │
//  │  [TODO - Gameplay Task 2] clampPosition() <- Eman friend  │
//  │  [TODO - Gameplay Task 1] onCollision()   <- Eman         │
//  │  [TODO - Gameplay Task 1] calculateArea() <- Eman         │
//  └──────────────────────────────────────────────────────────┘
// ============================================================

#include "Entity.h"
#include <stdexcept>
#include <sstream>

// ── Static member initialisation ──────────────────────────────────
int Entity::nextId = 0;

// ── Constructor ───────────────────────────────────────────────────
Entity::Entity(EntityType type, Vec2 startPos)
    : type(type)
    , position(startPos)
    , velocity({0, 0})
    , active(true)
    , health(1)          // Default 1 hit — Gameplay Task 1 adjusts per type
    , id(nextId++)
{}


// ================================================================
//  [ CORE TASK 2 — YOUR IMPLEMENTATION ]
//  display()
//
//  Returns the ASCII character to render this entity on the grid.
//
//  Design notes:
//   - Called once per entity per frame by Engine::render() (core.cpp)
//   - The Network team's serialize() embeds this char for state sync
//   - Do NOT add ANSI colour codes here — keep it pure ASCII
// ================================================================
char Entity::display() const {
    switch (type) {
        case EntityType::TOP_PLAYER:    return 'V';  // Inverted triangle tip = shoots down
        case EntityType::BOTTOM_PLAYER: return 'M';  // Ground player (legs = stable base)
        case EntityType::INSECT:        return 'X';  // Enemy obstacle
        case EntityType::PROJECTILE:    return '|';  // Falling bullet / point
        default:                        return '?';  // Unknown — should never appear
    }
}


// ================================================================
//  [ CORE TASK 2 — YOUR IMPLEMENTATION ]
//  serialize()
//
//  Packs entity state into a compact string for the Network team.
//  Format: "ID:TypeChar:X:Y:Active"
//
//  Examples:
//    "3:X:18:10:1"  -> Insect id=3, pos=(18,10), alive
//    "0:V:9:0:1"    -> Top Player, pos=(9,0), alive
//    "7:|:9:14:0"   -> Dead projectile
//
//  Network Team Task 2 calls this via EntityManager::serializeAll()
//  to broadcast world state to all connected clients each tick.
// ================================================================
std::string Entity::serialize() const {
    std::ostringstream ss;
    ss << id
       << ':' << display()          // Reuse display() so char is always consistent
       << ':' << position.x
       << ':' << position.y
       << ':' << (active ? 1 : 0);
    return ss.str();
}


// ================================================================
//  [ GAMEPLAY TASK 2 — Eman's friend implements this ]
//  move()
//
//  Apply `dir` to position according to role-based rules:
//    TOP_PLAYER    -> apply X only  (ceiling slide, no vertical)
//    BOTTOM_PLAYER -> apply X and Y (ground walk + jump impulse)
//    INSECT        -> apply X only  (scrolls left toward player)
//    PROJECTILE    -> apply Y only  (falls straight down)
//
//  IMPORTANT: Call clampPosition() at the end of this function.
//  Gravity for the Bottom Player: increment velocity.y by 1 each frame.
// ================================================================
void Entity::move(Vec2 dir) {
    // TODO: Gameplay Task 2 (Eman's friend) ─────────────────────────
    //
    // Suggested sketch:
    //
    //   const int GRAVITY = 1;
    //   switch (type) {
    //       case EntityType::TOP_PLAYER:
    //           position.x += dir.x;
    //           break;
    //       case EntityType::BOTTOM_PLAYER:
    //           velocity.x  = dir.x;
    //           velocity.y += GRAVITY;
    //           position    = position + velocity;
    //           break;
    //       case EntityType::INSECT:
    //           position.x += dir.x;
    //           break;
    //       case EntityType::PROJECTILE:
    //           position.y += dir.y;
    //           break;
    //   }
    //   clampPosition();
    // ────────────────────────────────────────────────────────────────
    (void)dir; // suppress unused-param warning until implemented
}


// ================================================================
//  [ GAMEPLAY TASK 2 — Eman's friend implements this ]
//  clampPosition()
//
//  Enforce the 20x20 arena hard limits: X in [0..19], Y in [0..19]
//
//  Special per-type rules:
//    TOP_PLAYER    -> always locked to row 0  (ceiling)
//    BOTTOM_PLAYER -> floor is row 19, stop velocity.y there
//    INSECT        -> x < 0 means it crossed left wall -> deactivate
//    PROJECTILE    -> y > 19 means it hit floor -> deactivate
// ================================================================
void Entity::clampPosition() {
    // TODO: Gameplay Task 2 (Eman's friend) ─────────────────────────
    //
    // Suggested sketch:
    //
    //   if (position.x < 0)  position.x = 0;
    //   if (position.x > 19) position.x = 19;
    //   if (position.y < 0)  position.y = 0;
    //   if (position.y > 19) position.y = 19;
    //
    //   if (type == EntityType::TOP_PLAYER)
    //       position.y = 0;
    //
    //   if (type == EntityType::BOTTOM_PLAYER && position.y >= 19) {
    //       position.y = 19;
    //       velocity.y = 0;
    //   }
    //   if (type == EntityType::INSECT && position.x < 0)
    //       active = false;
    //   if (type == EntityType::PROJECTILE && position.y >= 19)
    //       active = false;
    // ────────────────────────────────────────────────────────────────
}


// ================================================================
//  [ GAMEPLAY TASK 1 — Eman implements this ]
//  onCollision()
//
//  Called by PhysicsEngine::resolveCollision() when two entities
//  occupy the same grid cell.
//
//  Required behaviours:
//    Projectile  hits Insect        -> insect.takeDamage(1), proj off
//    Insect      hits Bottom Player -> bottomPlayer.takeDamage(1)
//    Insect      hits Top Player    -> (shouldn't happen — ceiling)
// ================================================================
void Entity::onCollision(Entity* other) {
    // TODO: Gameplay Task 1 (Eman) ───────────────────────────────────
    //
    // Suggested sketch:
    //
    //   if (type == EntityType::PROJECTILE &&
    //       other->getType() == EntityType::INSECT) {
    //       other->takeDamage(1);
    //       this->setActive(false);
    //       // Tell Gameplay Task 3 to update score here
    //   }
    //   if (type == EntityType::INSECT &&
    //       other->getType() == EntityType::BOTTOM_PLAYER) {
    //       other->takeDamage(1);
    //   }
    // ────────────────────────────────────────────────────────────────
    (void)other; // suppress warning until implemented
}


// ================================================================
//  [ GAMEPLAY TASK 1 — Eman implements this ]
//  calculateArea()
//
//  Returns the hit-box radius (grid cells) for collision detection.
//  PhysicsEngine uses this to determine overlap range.
//
//  Suggested values (discuss with Eman):
//    TOP_PLAYER    -> 3  (triangle spans 3 columns at ceiling)
//    BOTTOM_PLAYER -> 1  (single cell)
//    INSECT        -> 2  (box spans 2 cells wide)
//    PROJECTILE    -> 1  (single point)
// ================================================================
int Entity::calculateArea() const {
    // TODO: Gameplay Task 1 (Eman) ───────────────────────────────────
    return 1; // placeholder — replace with switch per type
}