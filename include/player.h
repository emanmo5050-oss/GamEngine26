// Player.h
// Task 2: Entity System

#pragma once
#include "Entity.h"
#include "PhysicsEngine.h"
#include <string>
#include <memory>
#include <vector>

class Projectile;

class Player : public Entity {
private:
    std::string username;
    int score = 0;

public:
    Player(int id, Vec2 pos, const std::string& name);

    void onCollision(Entity* other) override;

    virtual std::string serialize() const override;
    virtual void deserialize(const std::string& data) override;

    void move(Vec2 dir) override;
    std::unique_ptr<Projectile> shoot();

    int getScore() const { return score; }
    void addScore(int pts) { score += pts; }
    const std::string& getUsername() const { return username; }
};