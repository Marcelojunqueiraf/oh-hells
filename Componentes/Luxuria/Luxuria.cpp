#include "Luxuria.hpp"

#define PI 3.14159265359

#define WALK_SPEED 5

static Sprite *last_animation = nullptr;

Luxuria::Luxuria(std::weak_ptr<GameObject> associated, int hp) : Component(associated), hp(100)
{
    hit_animation = new Sprite("Assets/Luxuria_front_hit.png", associated, 6, 0.1);
    hit_animation->SetScaleX(3, 3);
    idle_animation = new Sprite("Assets/Luxuria_idle.png", associated, 5, 0.1);
    idle_animation->SetScaleX(3, 3);

    hit_animation->show = false;
    last_animation = idle_animation;
    associated.lock()->AddComponent(hit_animation);
    associated.lock()->AddComponent(idle_animation);

    shootCooldown = Timer();
    hitTimer = Timer();
}

Luxuria::~Luxuria()
{
}

void Luxuria::Update(float dt)
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

    InputManager &input = InputManager::GetInstance();

    if (shootCooldown.Get() > 1.5f)
    {
        GameObject *bulletGO = new GameObject();
        bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
        bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
        std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

        // get mouse direction
        Vec2 distance = Camera::GetInstance().pos + Vec2(512, 300) - this->associated.lock()->box.GetCenter();

        float angle = distance.getAngle();
        Bullet *bullet = new RegularBullet(bulletPtr, angle, 500, 10, 1000, "Assets/Luxuria_bullet.png", true);
        bulletGO->AddComponent(bullet);
        shootCooldown.Restart();
    }

    ShowSprite(idle_animation);
}

bool Luxuria::Is(std::string type)
{
    return type == "Luxuria";
}

void Luxuria::NotifyCollision(std::weak_ptr<GameObject> other)
{
    Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
    if (bullet != nullptr && !bullet->targetPlayer)
    {
        TakeDamage(bullet->GetDamage());
        ShowSprite(hit_animation);
        hitTimer.Restart();
    }
}

int Luxuria::GetHp()
{
    return hp;
}

void Luxuria::TakeDamage(int damage)
{
    if (hitTimer.Get() < 0.4f)
        return;
    std::cout << "Luxuria took " << damage << " damage" << std::endl;
    hp -= damage;
}

void Luxuria::ShowSprite(Sprite *spr)
{
    last_animation->show = false;
    spr->show = true;
    last_animation = spr;
}
