//Factory.cpp
#include "EntityFactory.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"
#include "PowerUp.h"
#include "Obstacle.h"
#include "Item.h"

std::unique_ptr<Entity> EntityFactory::createEntity(
    const std::string& type, int id, Vec2 position, const std::string& extra) 
{
    std::string t = type;
    // الـ Normalization اللي أخدناه من النسخة التالتة لسهولة التعامل
    if (t == "P")  t = "Player";
    if (t == "E")  t = "Enemy";
    if (t == "PR") t = "Projectile";
    if (t == "PU") t = "PowerUp";
    if (t == "O")  t = "Obstacle";
    if (t == "I")  t = "Item";

    if (t == "Player") return std::make_unique<Player>(id, position, extra.empty() ? "Player1" : extra);
    if (t == "Enemy")  return std::make_unique<Enemy>(id, position);
    if (t == "Projectile") return std::make_unique<Projectile>(id, position, Vec2(1,0)); 
    if (t == "PowerUp") return std::make_unique<PowerUp>(id, position);
    if (t == "Obstacle") return std::make_unique<Obstacle>(id, position);
    if (t == "Item") return std::make_unique<Item>(id, position);

    return nullptr;
}