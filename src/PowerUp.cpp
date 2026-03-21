// PowerUp.cpp
// Task 2: Entity System - Improved
#include "PowerUp.h"
#include <sstream>

PowerUp::PowerUp(int id, Vec2 pos, int val)
    : Entity(id, pos, '+', "PowerUp"), value(val) {}

void PowerUp::update(PhysicsEngine& physics) {
    // يمكن إضافة تأثير بصري بسيط لاحقاً
}

void PowerUp::render(std::vector<std::vector<char>>& grid) const {
    auto p = position;
    if (p.x >= 0 && p.x < 20 && p.y >= 0 && p.y < 20)
        grid[p.y][p.x] = symbol;
}

void PowerUp::onCollision(Entity* other) {
    if (other->getType() == "Player") {
        active = false;
    }
}

std::string PowerUp::serialize() const {
    std::stringstream ss;
    ss << "PU," << id << "," << position.x << "," << position.y << "," << value;
    return ss.str();
}

void PowerUp::deserialize(const std::string& data) {
    std::stringstream ss(data.substr(3));
    char c;
    ss >> id >> c >> position.x >> c >> position.y >> c >> value;
}