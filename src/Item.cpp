// Item.cpp
// Task 2: Entity System - Improved
#include "Item.h"
#include <sstream>

Item::Item(int id, Vec2 pos, int pts)
    : Entity(id, pos, 'I', "Item"), points(pts) {}

void Item::update(PhysicsEngine& physics) {}

void Item::render(std::vector<std::vector<char>>& grid) const {
    auto p = position;
    if (p.x >= 0 && p.x < 20 && p.y >= 0 && p.y < 20)
        grid[p.y][p.x] = symbol;
}

void Item::onCollision(Entity* other) {
    if (other->getType() == "Player") {
        active = false;
    }
}

std::string Item::serialize() const {
    std::stringstream ss;
    ss << "I," << id << "," << position.x << "," << position.y << "," << points;
    return ss.str();
}

void Item::deserialize(const std::string& data) {
    std::stringstream ss(data.substr(2));
    char c;
    ss >> id >> c >> position.x >> c >> position.y >> c >> points;
}