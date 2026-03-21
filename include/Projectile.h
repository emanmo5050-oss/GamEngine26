// Projectile.h
// Task 2: Entity System - Improved
#pragma once
#include "Entity.h"

class Projectile : public Entity {
private:
    int damage = 25;

public:
    Projectile(int id, Vec2 pos, Vec2 dir, int dmg = 25);

    void update(PhysicsEngine& physics) override;
    void render(std::vector<std::vector<char>>& grid) const override;
    void onCollision(Entity* other) override;

    std::string serialize() const override;
    void deserialize(const std::string& data) override;

    int getDamage() const { return damage; }
};