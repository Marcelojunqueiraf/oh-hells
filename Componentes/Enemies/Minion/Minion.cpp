// #include "Minion.hpp"

// Minion::Minion(std::weak_ptr<GameObject> associated) : Component(associated)
// {

//     hit_animation = new Sprite("Assets/Eli_front_hit.png", associated, 4, 0.1);
//     hit_animation->SetScaleX(3, 3);
//     idle_animation = new Sprite("Assets/Eli_front_idle.png", associated, 12, 0.1);
//     idle_animation->SetScaleX(3, 3);
//     hit_animation->show = false;
//     idle_animation->show = false;
//     last_animation = idle_animation;
//     associated.lock()->AddComponent(hit_animation);
//     associated.lock()->AddComponent(idle_animation);

//     enemy = new Enemy(associated, 50);
//     associated.lock()->AddComponent(enemy);
//     associated.lock()->AddComponent(new Collider(associated, {0.3, 0.3}, {64, 72}));

//     shootCooldown = Timer();
//     hitTimer = Timer();
// }

// Minion::~Minion()
// {
// }

// void Minion::Update(float dt)
// {

//     hitTimer.Update(dt);
//     shootCooldown.Update(dt);

//     if (hitTimer.Get() < 0.4f)
//     {
//         return;
//     }

//     if (this->enemy->GetHp() <= 0)
//     {
//         associated.lock()->RequestDelete();
//     }

//     last_animation->show = false;
//     idle_animation->show = true;
//     last_animation = idle_animation;

//     InputManager &input = InputManager::GetInstance();

//     if (shootCooldown.Get() > 1.5f)
//     {
//         GameObject *bulletGO = new GameObject();
//         bulletGO->box.x = this->associated.lock()->box.GetCenter().x;
//         bulletGO->box.y = this->associated.lock()->box.GetCenter().y;
//         std::weak_ptr<GameObject> bulletPtr = Game::GetInstance()->GetCurrentState().AddObject(bulletGO);

//         // get mouse direction
//         Vec2 distance = Camera::GetInstance().pos + Vec2(512, 300) - this->associated.lock()->box.GetCenter();

//         float angle = distance.getAngle();
//         Bullet *bullet = new Bullet(bulletPtr, angle, 500, 10, 1000, "Assets/minionbullet2.png", true);
//         bulletGO->AddComponent(bullet);
//         shootCooldown.Restart();
//     }
// }

// bool Minion::Is(std::string type)
// {
//     return type == "Minion";
// }

// void Minion::NotifyCollision(std::weak_ptr<GameObject> other)
// {
//     Bullet *bullet = (Bullet *)other.lock()->GetComponent("Bullet").lock().get();
//     if (bullet != nullptr && !bullet->targetPlayer)
//     {
//         last_animation->show = false;
//         hit_animation->show = true;
//         last_animation = hit_animation;
//         hitTimer.Restart();
//     }
// }