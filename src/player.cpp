#include "Player.h"
#include "Projectile.h"
#include <sstream>

Player::Player(int id, Vec2 pos, const std::string& name)
    : Entity(EntityType::BOTTOM_PLAYER, pos), username(name) {
    setHealth(3);  // Player starts with 3 health
}

void Player::onCollision(Entity* other) {
    if (!other || !other->isActive()) return;

    if (other->getType() == EntityType::INSECT) {
        takeDamage(1);
    }
}

void Player::move(Vec2 dir) {
    Entity::move(dir);
}

std::unique_ptr<Projectile> Player::shoot() {
    Vec2 bulletDir = {0, 1};  // shoots downward
    return std::make_unique<Projectile>(getId() * 100 + 1, position, bulletDir, 25);
}

std::string Player::serialize() const {
    std::ostringstream ss;
    ss << Entity::serialize() << ":" << username << ":" << score;
    return ss.str();
}

void Player::deserialize(const std::string& data) {
    Entity::deserialize(data);
}