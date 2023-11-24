#include "Enemy.hpp"

Enemy::Enemy(std::weak_ptr<GameObject> associated, int hp) : Component(associated), hp(100)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{
}

bool Enemy::Is(std::string type)
{
    return type == "Enemy";
}

void Enemy::NotifyCollision(std::weak_ptr<GameObject> other)
{
    Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
    if (bullet != nullptr && !bullet->targetPlayer)
    {
        hp -= bullet->GetDamage();
    }
}

int Enemy::GetHp()
{
    return hp;
}

void Enemy::TakeDamage(int damage)
{
    hp -= damage;
}