#pragma once

// ============================================================
//  Entity.h  —  Base Entity Class Declaration
//
//  ┌─────────────────────────────────────────────────────┐
//  │  WHO TOUCHES THIS FILE?                             │
//  │                                                     │
//  │  Core    Task 2  → implements display()   ← YOU    │
//  │  Gameplay Task 1 → implements onCollision()         │
//  │            Task 1 → implements calculateArea()      │
//  │            Task 2 → implements move(), clamp()      │
//  │            Task 4 → uses EntityFactory/Manager      │
//  │  Network  Task 2  → uses serialize()                │
//  └─────────────────────────────────────────────────────┘
//
//  Game Context:
//    TOP_PLAYER    = Inverted triangle on ceiling, slides L/R, shoots
//    BOTTOM_PLAYER = Ground player that jumps over insects
//    INSECT        = Enemy coming from the right side (boxes/obstacles)
//    PROJECTILE    = Bullet fired downward by the Top Player
// ============================================================

#include "Vec2.h"
#include <string>

// ------------------------------------------------------------------
//  EntityType  —  Determines symbol, behaviour, and physics rules
// ------------------------------------------------------------------
enum class EntityType {
    TOP_PLAYER,     // Rendered as 'V'  — ceiling, X-axis only
    BOTTOM_PLAYER,  // Rendered as 'M'  — ground, can jump
    INSECT,         // Rendered as 'X'  — enemy obstacle
    PROJECTILE      // Rendered as '|'  — bullet from Top Player
};

// ------------------------------------------------------------------
//  Entity  —  Abstract base for every object in the game world
// ------------------------------------------------------------------
class Entity {
public:
    // ── Construction / Destruction ─────────────────────────────────
    Entity(EntityType type, Vec2 startPos);
    virtual ~Entity() = default;

    // ── [ CORE TASK 2 — YOUR RESPONSIBILITY ] ─────────────────────
    //  Returns the single ASCII character that represents this entity
    //  on the 20x20 grid.
    //
    //  Mapping (do NOT change — Network team relies on these):
    //    TOP_PLAYER    → 'V'
    //    BOTTOM_PLAYER → 'M'
    //    INSECT        → 'X'
    //    PROJECTILE    → '|'
    virtual char display() const;

    // ── [ GAMEPLAY TASK 2 — Eman's friend's responsibility ] ──────
    //  Moves the entity by `dir` (restricted per entity type).
    //  Top Player:    only X component is applied
    //  Bottom Player: both X and Y (jump = negative Y impulse)
    virtual void move(Vec2 dir);

    //  Keeps position inside the 20x20 arena boundaries [0..19].
    void clampPosition();

    // ── [ GAMEPLAY TASK 1 — Eman's responsibility ] ───────────────
    //  Called when this entity physically overlaps another.
    virtual void onCollision(Entity* other);

    //  Returns the bounding-box area used for hit detection.
    virtual int calculateArea() const;

    // ── [ NETWORK TASK 2 — Network team's responsibility ] ────────
    //  Serialises this entity's state into "ID:Type:X:Y:Active"
    //  Used by serializeAll() in EntityManager.
    std::string serialize() const;
    void deserialize(const std::string& data);

    // ── Shared Getters (used by every team) ───────────────────────
    Vec2       getPosition()  const { return position; }
    EntityType getType()      const { return type;     }
    bool       isActive()     const { return active;   }
    int        getId()        const { return id;       }

    // ── Setters (used by Gameplay Task 4 for data security) ───────
    void setPosition(Vec2 pos) { position = pos; }
    void setActive(bool a)     { active = a;     }

    // Health getters/setters — Gameplay Task 4 enforces private access
    int  getHealth()          const { return health; }
    Vec2 getVelocity() const { return velocity; }
    void setHealth(int hp)          { health = hp;   }
    void takeDamage(int dmg)        { health -= dmg; if (health <= 0) active = false; }

protected:
    Vec2       position;
    Vec2       velocity;   // Used by Gameplay Task 2 for jump physics
    EntityType type;
    bool       active;

private:
    // ── Private Members (enforced by Gameplay Task 4) ─────────────
    int        health;
    int        id;

    static int nextId;     // Auto-incremented unique ID
};