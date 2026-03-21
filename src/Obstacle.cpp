// Obstacle.cpp
// Task 2: Entity System - Improved
#include "Obstacle.h"
#include <sstream>

Obstacle::Obstacle(int id, Vec2 pos) : Entity(id, pos, '#', "Obstacle") {}

void Obstacle::update(PhysicsEngine& physics) {} // ثابت

void Obstacle::render(std::vector<std::vector<char>>& grid) const {
    auto p = position;
    if (p.x >= 0 && p.x < 20 && p.y >= 0 && p.y < 20)
        grid[p.y][p.x] = symbol;
}

void Obstacle::onCollision(Entity* other) {
    if (other->getType() == "Player" || other->getType() == "Projectile") {
        other->takeDamage(15);
        // دفع اللاعب للخلف
        other->setPosition(other->getPosition() - other->getVelocity());
    }
}

std::string Obstacle::serialize() const {
    std::stringstream ss;
    ss << "O," << id << "," << position.x << "," << position.y;
    return ss.str();
}

void Obstacle::deserialize(const std::string& data) {
    std::stringstream ss(data.substr(2));
    char c;
    ss >> id >> c >> position.x >> c >> position.y;
}