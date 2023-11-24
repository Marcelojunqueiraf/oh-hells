#pragma once

#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../../Game/Game.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../Bullet/Bullet.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>

class Player : public Component
{

public:
    Player(std::weak_ptr<GameObject> associated);
    ~Player();
    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    static Player *player;
    void NotifyCollision(std::weak_ptr<GameObject> other);
    Vec2 Position();

private:
    Sprite *stand_straight;
    Sprite *walk_left, *walk_right, *walk_back, *walk_front, *hit_animation;
    Vec2 speed;
    float linearSpeed;
    float angle;
    int hp;
    Timer shootCooldown;
    Timer hitTimer;
};