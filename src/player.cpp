//Player.cpp
#include "Player.h"
#include "Projectile.h"
#include <sstream>

// ... الكود المعتاد ...

void Player::onCollision(Entity* other) {
    if (other->getType() == "PowerUp") {
        score += 50;
        heal(25); // أخدنا قيمة الـ Heal الأعلى من V1
    }
    else if (other->getType() == "Enemy") {
        takeDamage(20);
    }
}

std::unique_ptr<Projectile> Player::shoot() {
    Vec2 bulletDir = velocity;
    if (bulletDir.x == 0 && bulletDir.y == 0) bulletDir = {1, 0}; // Shooting default direction
    return std::make_unique<Projectile>(id * 100 + 1, position, bulletDir, 25);
}