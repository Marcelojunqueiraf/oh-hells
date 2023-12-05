#include "Preguica.hpp"

#define PI 3.14159265359

#define WALK_SPEED 100

static Sprite *last_animation = nullptr;

Vec2 positions[] = {
    Vec2(750, 300),
    Vec2(650, 400),
    Vec2(750, 500),
    Vec2(850, 400),
};

Preguica::Preguica(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go) : Component(associated),
                                                                                                        hp(5000), currentPosition(0), player_go(player_go), fase(ESPERANDO)
{
    // hit_animation = new Sprite("Assets/Luxuria_front_hit.png", associated, 6, 0.1);
    // hit_animation->SetScaleX(3, 3);
    idle_animation = new Sprite("Assets/Preguica_idle.png", associated);
    idle_animation->SetScaleX(3, 3);

    // hit_animation->show = false;
    last_animation = idle_animation;
    // associated.lock()->AddComponent(hit_animation);
    associated.lock()->AddComponent(idle_animation);

    shootCooldown = Timer();
    hitTimer = Timer();
}

Preguica::~Preguica()
{
}

void Preguica::Update(float dt)
{
    switch (fase)
    {
    case ESPERANDO:
    {
        Vec2 player_pos = player_go.lock()->box.GetCenter();
        Vec2 distance = player_pos - this->associated.lock()->box.GetCenter();
        if (hp < 4999)
        {
            hp = 5000;
            fase = CIRCULO;
        }
        break;
    }
    case CIRCULO:
    {
        shootCooldown.Update(dt);

        if (hp <= 0)
        {
            hp = 5000;
            fase = MINION;
            associated.lock()->box.x = 690;
            associated.lock()->box.y = 100;
            break;
        }

        if (shootCooldown.Get() > 1.5f)
        {
            if (!player_go.expired())
            {
                Vec2 player_pos = player_go.lock()->box.GetCenter();

                GameObject *bulletGO = new GameObject();
                // random angle
                float angle = (rand() % 360) * PI / 180;
                float radius = 400;
                Vec2 spawn = player_pos;
                spawn.x += radius * cos(angle);
                spawn.y += radius * sin(angle);

                bulletGO->box.x = spawn.x;
                bulletGO->box.y = spawn.y;

                std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

                // get mouse direction
                Vec2 distance = player_pos - spawn;

                angle = distance.getAngle();
                Bullet *bullet = new RegularBullet(bulletPtr, angle, 500, 10, 1000, "Assets/Luxuria_bullet.png", true);
                bulletGO->AddComponent(bullet);
                shootCooldown.Restart();
            }
        }

        ShowSprite(idle_animation);
        break;
    }

    case MINION:
    {
        shootCooldown.Update(dt);

        if (hp <= 0)
        {
            hp = 5000;
            fase = TIRO;
        }

        if (shootCooldown.Get() > 2.5f)
        {
            Vec2 player_pos = player_go.lock()->box.GetCenter();

            // random angle
            float angle = (rand() % 360) * PI / 180;
            float radius = 200;
            Vec2 spawn = player_pos;
            spawn.x += radius * cos(angle);
            spawn.y += radius * sin(angle);

            GameObject *minion = new GameObject();
            minion->box.x = spawn.x;
            minion->box.y = spawn.y;
            std::weak_ptr<GameObject> minionPtr = Game::GetInstance()->GetCurrentState().AddObject(minion);
            Minion *minionComponent = new Minion(minionPtr, 100, player_go);

            minion->AddComponent(minionComponent);

            shootCooldown.Restart();
        }
        break;
    }

    case TIRO:
    {

        moveCooldown.Update(dt);
        shootCooldown.Update(dt);

        if (moveCooldown.Get() > 5)
        {
            moveCooldown.Restart();
            currentPosition = (currentPosition + 1) % 4;
        }

        if (hp <= 0)
        {
            associated.lock()->RequestDelete();
        }

        InputManager &input = InputManager::GetInstance();

        // calculate distance to target position
        Vec2 target = positions[currentPosition];
        Vec2 distance = target - associated.lock()->box.GetCenter();
        if (distance.magnitude() > WALK_SPEED * dt)
        {
            Vec2 velocity = distance.normalize() * WALK_SPEED * dt;
            associated.lock()->box.x += velocity.x;
            associated.lock()->box.y += velocity.y;
        }

        if (shootCooldown.Get() > 1.5f)
        {
            if (!player_go.expired())
            {
                GameObject *bulletGO = new GameObject();
                bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
                bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
                std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

                // get mouse direction
                Vec2 player_pos = player_go.lock()->box.GetCenter();
                Vec2 distance = player_pos - this->associated.lock()->box.GetCenter();

                float angle = distance.getAngle();
                Bullet *bullet = new RegularBullet(bulletPtr, angle, 500, 10, 1000, "Assets/Luxuria_bullet.png", true);
                bulletGO->AddComponent(bullet);
                shootCooldown.Restart();
            }
        }

        ShowSprite(idle_animation);
    }
    break;
    }
}

bool Preguica::Is(std::string type)
{
    return type == "Preguica";
}

void Preguica::NotifyCollision(std::weak_ptr<GameObject> other)
{
    Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
    if (bullet != nullptr && !bullet->targetPlayer)
    {
        hp -= bullet->GetDamage();
        // ShowSprite(hit_animation);
        hitTimer.Restart();
    }
}

int &Preguica::GetHp()
{
    return hp;
}

void Preguica::TakeDamage(int damage)
{
    hp -= damage;
}

void Preguica::ShowSprite(Sprite *spr)
{
    last_animation->show = false;
    spr->show = true;
    last_animation = spr;
}
