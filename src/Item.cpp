#include "Item.h"

Item::Item(int id, Vec2 pos, int pts)
    : Entity(EntityType::INSECT, pos), points(pts) {
    setHealth(1);
}

void Item::move(Vec2 dir) {
    (void)dir;
    // Items are static — don't move
}

void Item::onCollision(Entity* other) {
    if (!other || !other->isActive()) return;

    if (other->getType() == EntityType::BOTTOM_PLAYER) {
        setActive(false);
    }
}

std::string Item::serialize() const {
    return Entity::serialize();
}

void Item::deserialize(const std::string& data) {
    Entity::deserialize(data);
}