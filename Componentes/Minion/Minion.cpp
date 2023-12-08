#include "Minion.hpp"

#define PI 3.14159265359

#define WALK_SPEED 70

Minion::Minion(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go) : Component(associated),
                                                                                                    hp(hp), player_go(player_go)
{
    idle_animation = new Sprite("Assets/minion_preguica_idle.png", associated, 8, 0.1f);
    idle_animation->SetScaleX(2, 2);
    associated.lock()->AddComponent(idle_animation);
    idle_animation->show = true;

    associated.lock()->AddComponent(new HealthBar(associated, this->GetHp(), this->GetHp()));
    associated.lock()->AddComponent(new Collider(associated, {0.3, 0.3}, {64, 72}));

    shootCooldown = Timer();
    hitTimer = Timer();
}

Minion::~Minion()
{
}

void Minion::Update(float dt)
{
    shootCooldown.Update(dt);

    if (hp <= 0)
    {
        associated.lock()->RequestDelete();
    }

    if (!player_go.expired())
    {

    if (shootCooldown.Get() > 1.7f)
    {
            GameObject *bulletGO = new GameObject();
            bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
            bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
            std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

            Vec2 player_pos = player_go.lock()->box.GetCenter();
            Vec2 distance = player_pos - this->associated.lock()->box.GetCenter();

            float angle = distance.getAngle();
            Bullet *bullet = new RegularBullet(bulletPtr, angle, 300, 1, 1000, "Assets/Luxuria_bullet.png", true);
            bulletGO->AddComponent(bullet);
            shootCooldown.Restart();
    }

    Vec2 distance = player_go.lock()->box.GetCenter() - this->associated.lock()->box.GetCenter();
    if (distance.magnitude() > 150)
    {
        Vec2 velocity = distance.normalize() * WALK_SPEED * dt;
        associated.lock()->box.x += velocity.x;
        associated.lock()->box.y += velocity.y;
    }
    }

    ShowSprite(idle_animation);
}

bool Minion::Is(std::string type)
{
    return type == "Minion";
}

void Minion::NotifyCollision(std::weak_ptr<GameObject> other)
{
    Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
    if (bullet != nullptr && !bullet->targetPlayer)
    {
        hp -= bullet->GetDamage();
        // ShowSprite(hit_animation);
        hitTimer.Restart();
    }
}

int &Minion::GetHp()
{
    return hp;
}

void Minion::TakeDamage(int damage)
{
    hp -= damage;
}

void Minion::ShowSprite(Sprite *spr)
{
    spr->show = true;
}
