// Projectile.cpp
// Task 2: Entity System - Improved
#include "Projectile.h"
#include <sstream>

Projectile::Projectile(int id, Vec2 pos, Vec2 dir, int dmg)
    : Entity(id, pos, '*', "Projectile"), damage(dmg) {
    velocity = dir;
}

void Projectile::update(PhysicsEngine& physics) {
    position = position + velocity;

    if (position.x < 0 || position.x >= 20 || position.y < 0 || position.y >= 20)
        active = false;
}

void Projectile::render(std::vector<std::vector<char>>& grid) const {
    auto p = position;
    if (p.x >= 0 && p.x < 20 && p.y >= 0 && p.y < 20)
        grid[p.y][p.x] = symbol;
}

void Projectile::onCollision(Entity* other) {
    if (other->getType() == "Enemy" || other->getType() == "Obstacle")
        active = false;
}

std::string Projectile::serialize() const {
    std::stringstream ss;
    ss << "PR," << id << "," << position.x << "," << position.y << ","
       << health << "," << damage;
    return ss.str();
}

void Projectile::deserialize(const std::string& data) {
    std::stringstream ss(data.substr(3));
    char c;
    ss >> id >> c >> position.x >> c >> position.y >> c >> health >> c >> damage;
}