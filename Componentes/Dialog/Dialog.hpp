#pragma once

#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../../Game/Game.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>

class Dialog : public Component
{

public:
    enum DialogState
    {
        STARTING,
    };
    DialogState state = STARTING;
    Dialog(std::weak_ptr<GameObject> associated);
    ~Dialog();
    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    Vec2 Position();
    void ShowSprite(Sprite *spr);

private:
    Sprite *background;
    Timer keyCooldown;
    bool key_pressed;
};