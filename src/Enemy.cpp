// Enemy.cpp
// Task 2: Entity System - Improved
#include "Enemy.h"
#include "PhysicsEngine.h"
#include <sstream>
#include <cstdlib>
#include <ctime>

Enemy::Enemy(int id, Vec2 pos) : Entity(id, pos, 'E', "Enemy") {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Enemy::update(PhysicsEngine& physics) {
    velocity = {(rand() % 3) - 1, (rand() % 3) - 1};
    position = position + velocity;
}

void Enemy::render(std::vector<std::vector<char>>& grid) const {
    auto p = position;
    if (p.x >= 0 && p.x < 20 && p.y >= 0 && p.y < 20)
        grid[p.y][p.x] = symbol;
}

void Enemy::onCollision(Entity* other) {
    if (other->getType() == "Player" || other->getType() == "Projectile")
        takeDamage(30);
}

std::string Enemy::serialize() const {
    std::stringstream ss;
    ss << "E," << id << "," << position.x << "," << position.y << "," << health;
    return ss.str();
}

void Enemy::deserialize(const std::string& data) {
    std::stringstream ss(data.substr(2));
    char c;
    ss >> id >> c >> position.x >> c >> position.y >> c >> health;
}