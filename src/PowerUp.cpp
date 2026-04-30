#include "PowerUp.h"
#include "core.h"

PowerUp::PowerUp(int id, Vec2 pos, int ammo)
    : Entity(EntityType::INSECT, pos), ammoValue(ammo) {
    setHealth(1);
}

void PowerUp::move(Vec2 dir) {
    (void)dir;
    // PowerUp slowly falls down
    velocity = {0, 1};
    position = position + velocity;
    clampPosition();
    // Deactivate if it reaches the floor
    if (getPosition().y >= 18) setActive(false);
}

void PowerUp::onCollision(Entity* other) {
    if (!other || !other->isActive()) return;

    if (other->getType() == EntityType::BOTTOM_PLAYER) {
        if (auto* engine = Engine::getInstance()) {
            engine->depositAmmo(ammoValue);
            engine->addScore(10);
        }
        setActive(false);
    }
}

std::string PowerUp::serialize() const {
    return Entity::serialize();
}

void PowerUp::deserialize(const std::string& data) {
    Entity::deserialize(data);
}