#include "Entity.h"
#include <stdexcept>
#include <sstream>
 
int Entity::nextId = 0;
 
Entity::Entity(EntityType type, Vec2 startPos)
    : type(type)
    , position(startPos)
    , velocity({0, 0})
    , active(true)
    , health(1)
    , id(nextId++)
{}
 
// [ CORE TASK 2 — display() ]
char Entity::display() const {
    switch (type) {
        case EntityType::TOP_PLAYER:    return 'V';
        case EntityType::BOTTOM_PLAYER: return 'M';
        case EntityType::INSECT:        return 'X';
        case EntityType::PROJECTILE:    return '|';
        default:                        return '?';
    }
}
 
// [ CORE TASK 2 — serialize() ]
std::string Entity::serialize() const {
    std::ostringstream ss;
    ss << id << ':' << display() << ':' << position.x << ':' << position.y << ':' << (active ? 1 : 0);
    return ss.str();
}
 
// [ GAMEPLAY TASK 2 — move() — Yassin Athula implements this ]
void Entity::move(Vec2 dir) {
    (void)dir;
}
 
// [ GAMEPLAY TASK 2 — clampPosition() — Yassin Athula implements this ]
void Entity::clampPosition() {}
 
// [ GAMEPLAY TASK 1 — onCollision() — Eman ]
void Entity::onCollision(Entity* other) {
    if (!other || !other->isActive()) return;
 
    if (type == EntityType::PROJECTILE && other->getType() == EntityType::INSECT) {
        other->takeDamage(1);
        setActive(false);
        return;
    }
 
    if (type == EntityType::INSECT && other->getType() == EntityType::BOTTOM_PLAYER) {
        other->takeDamage(1);
        return;
    }
}
 
// [ GAMEPLAY TASK 1 — calculateArea() — Eman ]
int Entity::calculateArea() const {
    switch (type) {
        case EntityType::TOP_PLAYER:    return 3;
        case EntityType::BOTTOM_PLAYER: return 1;
        case EntityType::INSECT:        return 2;
        case EntityType::PROJECTILE:    return 1;
        default:                        return 1;
    }
}
void Entity::deserialize(const std::string& data) {
    std::stringstream ss(data);
    std::string idStr, typeChar, xStr, yStr, activeStr;
    std::getline(ss, idStr, ':');
    std::getline(ss, typeChar, ':');
    std::getline(ss, xStr, ':');
    std::getline(ss, yStr, ':');
    std::getline(ss, activeStr, ':');
    position.x = std::stoi(xStr);
    position.y = std::stoi(yStr);
    active = (activeStr == "1");
}