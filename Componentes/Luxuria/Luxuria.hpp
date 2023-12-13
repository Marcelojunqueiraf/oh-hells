#pragma once

#include "../../Game/Game.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../Dialog/Dialog.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../RegularBullet/RegularBullet.hpp"
#include "../GuidedBullet/GuidedBullet.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>

class Sprite;

class Luxuria : public Component
{

public:
    enum LuxuriaState
    {
        RESTING,
        MOVING,
        SHOOTING,
        HITED,
        DEAD
    };
    LuxuriaState state = RESTING;
    Sprite *health_bar;
    Luxuria(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go);
    ~Luxuria();
    void Update(float dt);
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    void ShowSprite(Sprite *spr);
    int &GetHp();
    void TakeDamage(int damage);

private:
    int hp;
    std::weak_ptr<GameObject> player_go;
    bool music_playing = false;

    Sprite *idle_animation, *hit_animation, *shooting_animation;
    Timer shootCooldown;
    Timer hitTimer;
    Timer falasTimer;
};