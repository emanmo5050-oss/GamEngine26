// Item.h
// Task 2: Entity System - Improved
#pragma once
#include "Entity.h"

class Item : public Entity {
private:
    int points = 30;

public:
    Item(int id, Vec2 pos, int pts = 30);

    void update(PhysicsEngine& physics) override;
    void render(std::vector<std::vector<char>>& grid) const override;
    void onCollision(Entity* other) override;

    std::string serialize() const override;
    void deserialize(const std::string& data) override;
};