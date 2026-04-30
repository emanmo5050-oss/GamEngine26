#pragma once
#include "Entity.h"

class PowerUp : public Entity {
private:
    int ammoValue = 6;

public:
    PowerUp(int id, Vec2 pos, int ammo = 6);

    void move(Vec2 dir) override;
    void onCollision(Entity* other) override;

    virtual std::string serialize() const override;
    virtual void deserialize(const std::string& data) override;

    int getAmmoValue() const { return ammoValue; }
};