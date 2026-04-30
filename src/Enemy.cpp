#include "Enemy.h"
#include <cstdlib>

Enemy::Enemy(int id, Vec2 pos)
    : Entity(EntityType::INSECT, pos) {
    setHealth(1);
}

void Enemy::move(Vec2 dir) {
    (void)dir;
    // Enemy always moves left, slight random vertical movement
    velocity = {-1, (std::rand() % 3) - 1};
    position = position + velocity;
    clampPosition();
}

void Enemy::onCollision(Entity* other) {
    if (!other || !other->isActive()) return;

    if (other->getType() == EntityType::PROJECTILE) {
        takeDamage(1);
    }
    if (other->getType() == EntityType::BOTTOM_PLAYER) {
        other->takeDamage(1);
    }
}

std::string Enemy::serialize() const {
    return Entity::serialize();
}

void Enemy::deserialize(const std::string& data) {
    Entity::deserialize(data);
}