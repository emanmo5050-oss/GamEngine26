// Entity.h
// Task 2: Entity System

#pragma once
#include "Vec2.h"
#include <string>
#include <vector>

class PhysicsEngine;

class Entity {
protected:
    int id;
    Vec2 position;
    Vec2 velocity;
    int health = 100;
    char symbol;
    bool active = true;
    std::string type;

public:
    Entity(int id, Vec2 pos, char sym, const std::string& t);
    virtual ~Entity() = default;

    virtual void update(PhysicsEngine& physics) = 0;
    virtual void render(std::vector<std::vector<char>>& grid) const = 0;
    virtual void onCollision(Entity* other) = 0;

    virtual std::string serialize() const = 0;
    virtual void deserialize(const std::string& data) = 0;

    int getId() const { return id; }
    Vec2 getPosition() const { return position; }
    Vec2 getVelocity() const { return velocity; }
    int getHealth() const { return health; }
    bool isActive() const { return active; }
    char getSymbol() const { return symbol; }
    std::string getType() const { return type; }

    void setPosition(Vec2 p) { position = p; }
    void setVelocity(Vec2 v) { velocity = v; }

    void takeDamage(int dmg);
    void heal(int amount);

protected:
    void clampPosition();
};