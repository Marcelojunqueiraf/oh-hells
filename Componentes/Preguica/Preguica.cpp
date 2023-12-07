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
                                                                                                        hp(5000), currentPosition(0), player_go(player_go), fase(ESPERANDO), shootAngle(0)
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
    dialogTimer = Timer();

    Game::SetDialog("Assets/preguica_dialog.png", "Preguica", "Oh, tem alguem aqui");
    Game::ShowDialog(true);
}

Preguica::~Preguica()
{
}

void Preguica::Update(float dt)
{
    dialogTimer.Update(dt);

    switch (fase)
    {
    case ESPERANDO:
    {
        if (dialogTimer.Get() > 6)
        {
            Game::ShowDialog(false);
        }
        else if (dialogTimer.Get() > 3 && dialogTimer.Get() < 3.2f)
        {
            Game::SetDialog("Assets/preguica_dialog.png", "Preguica", "Tem como esperar? estou tirando um cochilo");
        }

        if (!player_go.expired())
        {
        Vec2 player_pos = player_go.lock()->box.GetCenter();
        Vec2 distance = player_pos - this->associated.lock()->box.GetCenter();
        }
        if (hp < 4999)
        {
            hp = 5000;
            fase = CIRCULO;
            dialogTimer.Restart();
            Game::SetDialog("Assets/preguica_dialog.png", "Preguica", "Que deselegante, me acordou!");
            Game::ShowDialog(true);
        }
        break;
    }
    case CIRCULO:
    {
        shootCooldown.Update(dt);

        if (dialogTimer.Get() > 2)
        {
            Game::ShowDialog(false);
        }

        if (hp <= 0)
        {
            hp = 5000;
            fase = MINION;
            associated.lock()->box.x = 690;
            associated.lock()->box.y = 100;

            dialogTimer.Restart();
            Game::SetDialog("Assets/preguica_dialog.png", "Preguica", "Minions, acabem com ele!");
            Game::ShowDialog(true);

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

        if (dialogTimer.Get() > 3)
        {
            Game::ShowDialog(false);
        }

        if (hp <= 0)
        {
            hp = 5000;
            fase = TIRO;

            dialogTimer.Restart();
            Game::SetDialog("Assets/preguica_dialog.png", "Preguica", "Se quer algo bem feito, faca voce mesmo!");
            Game::ShowDialog(true);

            break;
        }

        if (shootCooldown.Get() > 2.5f)
        {
            if (!player_go.expired())
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
        }
        break;
    }

    case TIRO:
    {

        if (dialogTimer.Get() > 2)
        {
            Game::ShowDialog(false);
        }

        moveCooldown.Update(dt);
        shootCooldown.Update(dt);

        if (moveCooldown.Get() > 5)
        {
            moveCooldown.Restart();
            currentPosition = (currentPosition + 1) % 4;

            break;
        }

        if (hp <= 0)
        {
            fase = MORRENDO;
            hp = 5000;
            dialogTimer.Restart();
            Game::SetDialog("Assets/preguica_dialog.png", "Preguica", "Nunca vou te perdoaaaaar...");
            Game::ShowDialog(true);
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
        break;
    }
    case MORRENDO:
    {
        if (hp <= 0)
        {
            Game::ShowDialog(false);
            associated.lock()->RequestDelete();
        }
        associated.lock()->angle += 2 * M_PI * dt;
        Sprite *playerSprite = (Sprite *)associated.lock()->GetComponent("Sprite").lock().get();
        Vec2 currentScale = playerSprite->GetScale();
        playerSprite->SetScaleX(currentScale.x * 0.99, currentScale.y * 0.99);

        hp -= 2000 * dt;
        break;
    }
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
