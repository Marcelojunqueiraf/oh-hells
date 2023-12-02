#pragma once

#include "../../Game/Game.hpp"
#include "../../States/State/State.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../Bullet/Bullet.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>

class ActionCollider : public Component
{

public:
    ActionCollider(std::weak_ptr<GameObject> associated, Vec2 scale, Vec2 offset, State* state, void (*callbak)(State*, std::weak_ptr<GameObject>));
    ~ActionCollider();
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);

private:
    State * state;
    void (*callbak)(State*, std::weak_ptr<GameObject>);
};