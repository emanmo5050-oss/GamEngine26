// Enemy.h
// Task 2: Entity System - Improved
#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(int id, Vec2 pos);

    void update(PhysicsEngine& physics) override;
    void render(std::vector<std::vector<char>>& grid) const override;
    void onCollision(Entity* other) override;

    std::string serialize() const override;
    void deserialize(const std::string& data) override;
};