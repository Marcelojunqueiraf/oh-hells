#include "Enemy.hpp"

#define PI 3.14159265359

#define WALK_SPEED 5

static Sprite *last_animation = nullptr;

Enemy::Enemy(std::weak_ptr<GameObject> associated, int hp) : Component(associated), hp(100)
{

    hit_animation = new Sprite("Assets/Eli_front_hit.png", associated, 4, 0.1);
    hit_animation->SetScaleX(3, 3);
    idle_animation = new Sprite("Assets/Eli_front_idle.png", associated, 12, 0.1);
    idle_animation->SetScaleX(3, 3);

    hit_animation->show = false;
    idle_animation->show = false;
    last_animation = idle_animation;
    associated.lock()->AddComponent(hit_animation);
    associated.lock()->AddComponent(idle_animation);

    shootCooldown = Timer();
    hitTimer = Timer();
}

Enemy::~Enemy()
{
}

void Enemy::Update(float dt)
{

    hitTimer.Update(dt);
    shootCooldown.Update(dt);

    if (hitTimer.Get() < 0.4f)
    {
        return;
    }

    if (hp <= 0)
    {
        associated.lock()->RequestDelete();
    }

    last_animation->show = false;
    idle_animation->show = true;
    last_animation = idle_animation;

    InputManager &input = InputManager::GetInstance();

    if (shootCooldown.Get() > 1.5f)
    {
        GameObject *bulletGO = new GameObject();
        bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
        bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
        std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().lock()->AddObject(bulletGO);

        // get mouse direction
        Vec2 distance = Camera::GetInstance().pos + Vec2(512, 300) - this->associated.lock()->box.GetCenter();

        float angle = distance.getAngle();
        Bullet *bullet = new Bullet(bulletPtr, angle, 500, 10, 1000, "Assets/minionbullet2.png", true);
        bulletGO->AddComponent(bullet);
        shootCooldown.Restart();
    }
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
        last_animation->show = false;
        hit_animation->show = true;
        last_animation = hit_animation;
        hitTimer.Restart();
    }
}