#include "Enemy.hpp"

#define PI 3.14159265359

#define WALK_SPEED 5

static Sprite *last_animation = nullptr;

Enemy::Enemy(std::weak_ptr<GameObject> associated, int hp) : Component(associated), hp(100)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{
    if (hp <= 0)
    {
        associated.lock()->RequestDelete();
    }
}

bool Enemy::Is(std::string type)
{
    return type == "Enemy";
}

void Enemy::NotifyCollision(std::weak_ptr<GameObject> other)
{
    Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
    if (bullet != nullptr)
    {
        hp -= bullet->GetDamage();
    }
}