// Entity.cpp
// Task 2: Entity System

#include "Entity.h"
#include <iostream>

Entity::Entity(int id, Vec2 pos, char sym, const std::string& t)
    : id(id), position(pos), symbol(sym), type(t) {
    velocity = {0, 0};
}

void Entity::takeDamage(int dmg) {
    health -= dmg;
    if (health <= 0) {
        active = false;
        std::cout << "[Entity " << id << " (" << type << ")] Destroyed!\n";
    }
}

void Entity::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;
}

void Entity::clampPosition() {
    if (position.x < 0) position.x = 0;
    if (position.x >= 20) position.x = 19;
    if (position.y < 0) position.y = 0;
    if (position.y >= 20) position.y = 19;
}