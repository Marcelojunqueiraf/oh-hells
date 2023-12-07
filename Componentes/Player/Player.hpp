#pragma once

#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../../Game/Game.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../Bullet/Bullet.hpp"
#include "../Boomerang/Boomerang.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>

class Player : public Component
{

public:
    enum PlayerState
    {
        RESTING,
        MOVING,
        SHOOTING,
        DASHING,
        HITED,
        DEAD
    };
    PlayerState state = RESTING;
    Player(std::weak_ptr<GameObject> associated);
    ~Player();
    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    Vec2 Position();
    void ShowSprite(Sprite *spr);
    void SetView(Rect max_view);
    void SetPosition(int x, int y);
    int &GetHp();

private:
    Rect max_view;
    Sprite *last_animation = nullptr;
    Sprite *stand_straight, *death_animation;
    Sprite *walk_left, *walk_right, *walk_back, *walk_front, *hit_animation;
    Sprite *left_attack_animation, *right_attack_animation, *front_attack_animation, *back_attack_animation;
    Sound *hit_sound, *sword_attack_sound;
    Vec2 speed;
    float linearSpeed;
    float angle;
    int hp;
    Timer shootCooldown;
    Timer hitTimer;

    bool dashed;
    bool died;
    Timer attackCooldown, dashTimer;
    Vec2 dash_direction;
};