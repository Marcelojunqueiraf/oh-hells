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

class Enemy : public Component
{

public:
    Enemy(std::weak_ptr<GameObject> associated, int hp);
    ~Enemy();
    void Update(float dt);
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);

private:
    int hp;
};