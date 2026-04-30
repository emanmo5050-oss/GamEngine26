#pragma once
#include "Entity.h"

class Item : public Entity {
private:
    int points = 10;

public:
    Item(int id, Vec2 pos, int pts = 10);

    void move(Vec2 dir) override;
    void onCollision(Entity* other) override;

    virtual std::string serialize() const override;
    virtual void deserialize(const std::string& data) override;

    int getPoints() const { return points; }
};