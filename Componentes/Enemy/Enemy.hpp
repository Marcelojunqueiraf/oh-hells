#pragma once

#include "../Component/Component.hpp"
#include "../../Game/Game.hpp"
#include "../Bullet/Bullet.hpp"

#include <memory>

class Sprite;

class Enemy : public Component
{

public:
    Enemy(std::weak_ptr<GameObject> associated, int hp);
    ~Enemy();
    void Update(float dt);
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    int GetHp();
    void TakeDamage(int damage);

private:
    int hp;
};