#include "Luxuria.hpp"
#include "../HealthBar/HealthBar.hpp"

#define PI 3.14159265359

#define WALK_SPEED 5

static Sprite *last_animation = nullptr;

#define SCALE 3
#define IMG_SIZE SCALE * 64

Luxuria::Luxuria(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go) : Component(associated), hp(5000), player_go(player_go)
{
    hit_animation = new Sprite("Assets/Luxuria_front_hit.png", associated, 6, 0.1);
    hit_animation->SetScaleX(3, 3);
    idle_animation = new Sprite("Assets/Luxuria_idle.png", associated, 5, 0.1);
    idle_animation->SetScaleX(3, 3);
    shooting_animation = new Sprite("Assets/Luxuria_front_attack.png", associated, 6, 0.1);
    shooting_animation->SetScaleX(3, 3);

    associated.lock()->AddComponent(hit_animation);
    associated.lock()->AddComponent(idle_animation);
    associated.lock()->AddComponent(shooting_animation);
    // associated.lock()->AddComponent(new HealthBar("Assets/barra_inimiga.png", associated, hp));

    shooting_animation->show = false;
    hit_animation->show = false;
    last_animation = idle_animation;

    Game::SetDialog("Assets/luxuria_dialog.png", "Luxúria", "Ah, o meu convidado especial chegou! Fique à vontade. Quer um drink ou algo do tipo?");
    Game::ShowDialog(true);
}

Luxuria::~Luxuria()
{
}

void Luxuria::Update(float dt)
{
    hitTimer.Update(dt);
    shootCooldown.Update(dt);
    falasTimer.Update(dt);

    if (falasTimer.Get() > 3)
    {
        Game::ShowDialog(false);
    }

    if (falasTimer.Get() > 15)
    {
        Game::SetDialog("Assets/luxuria_dialog2.png", "Luxúria", "Não vou perder para voce. Afinal, não posso aceitar que a escala 6x1 de trabalho volte");
        Game::ShowDialog(true);
    }

    if (falasTimer.Get() > 18)
    {
        Game::ShowDialog(false);
    }

    if (falasTimer.Get() > 30)
    {
        Game::SetDialog("Assets/luxuria_dialog1.png", "Luxúria", "Sinto muito querido. Se quiser seu amuleto vai ter que pegar a força. Muita forca >:)");
        Game::ShowDialog(true);
    }

    if (falasTimer.Get() > 33)
    {
        Game::ShowDialog(false);
    }

    if (hitTimer.Get() < 0.4f)
    {
        return;
    }

    if (hp <= 0)
    {
        associated.lock()->RequestDelete();
        return;
    }

    InputManager &input = InputManager::GetInstance();

    switch (state)
    {
    case (RESTING):
        if (shootCooldown.Get() > 1.5f)
        {
            if (!player_go.expired())
            {
                state = SHOOTING;
                ShowSprite(shooting_animation);
                GameObject *bulletGO = new GameObject();
                bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
                bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
                std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

                // get mouse direction
                Vec2 player_pos = player_go.lock()->box.GetCenter();
                Vec2 distance = player_pos - this->associated.lock()->box.GetCenter();

                float angle = distance.getAngle();
                bulletGO->angle = angle;

                Bullet *bullet = new GuidedBullet(bulletPtr, player_go, 100, 10, 1000, "Assets/Luxuria_bullet.png", true);
                // Bullet *bullet = new RegularBullet(bulletPtr, angle, 500, 10, 1000, "Assets/Luxuria_bullet.png", true);
                bulletGO->AddComponent(bullet);
                shootCooldown.Restart();
            }
        }
        break;
    case (SHOOTING):
        if (shootCooldown.Get() > 0.59f)
        {
            state = RESTING;
            shootCooldown.Restart();
            ShowSprite(idle_animation);
        }
        break;
    default:
        ShowSprite(idle_animation);
        break;
    }
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

int &Luxuria::GetHp()
{
    return hp;
}

void Luxuria::TakeDamage(int damage)
{
    hp -= damage;
}

void Luxuria::ShowSprite(Sprite *spr)
{
    last_animation->show = false;
    spr->show = true;
    last_animation = spr;
}