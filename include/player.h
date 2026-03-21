// Player.h
// Task 2: Entity System

#pragma once
#include "Entity.h"
#include <string>
#include <memory>

class Projectile;

class Player : public Entity {
private:
    std::string username;
    int score = 0;

public:
    Player(int id, Vec2 pos, const std::string& name);

    void update(PhysicsEngine& physics) override;
    void render(std::vector<std::vector<char>>& grid) const override;
    void onCollision(Entity* other) override;

    std::string serialize() const override;
    void deserialize(const std::string& data) override;

    void move(Vec2 dir);
    std::unique_ptr<Projectile> shoot();

    int getScore() const { return score; }
    const std::string& getUsername() const { return username; }
};