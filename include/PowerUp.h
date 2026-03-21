// PowerUp.h
// Task 2: Entity System - Improved
#pragma once
#include "Entity.h"

class PowerUp : public Entity {
private:
    int value = 50;

public:
    PowerUp(int id, Vec2 pos, int val = 50);

    void update(PhysicsEngine& physics) override;
    void render(std::vector<std::vector<char>>& grid) const override;
    void onCollision(Entity* other) override;

    std::string serialize() const override;
    void deserialize(const std::string& data) override;
};