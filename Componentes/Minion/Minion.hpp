#pragma once

#include "../../Game/Game.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../RegularBullet/RegularBullet.hpp"
#include "../../Utils/Timer/Timer.hpp"
#include "../HealthBar/HealthBar.hpp"

#include <memory>

class Sprite;

class Minion : public Component
{

public:
    Minion(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go);
    ~Minion();
    void Update(float dt);
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    void ShowSprite(Sprite *spr);
    int &GetHp();
    void TakeDamage(int damage);

private:
    int hp;
    std::weak_ptr<GameObject> player_go;
    Timer shootCooldown;
    Timer hitTimer;
    Sprite *idle_animation, *hit_animation;
};