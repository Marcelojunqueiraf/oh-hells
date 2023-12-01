#pragma once

#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Text/Text.hpp"
#include "../Sound/Sound.hpp"
#include "../../Game/Game.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../../Utils/Timer/Timer.hpp"

#include <memory>
#include <string_view>

struct dialog_info{
    // Sprite * character_emotion;
    std::string_view character_name;
    std::string_view character_msg;
};

class Dialog : public Component
{

public:
    Dialog(std::weak_ptr<GameObject> associated);
    ~Dialog();
    void Start();
    void Update(float dt);
    void Render();
    bool Is(std::string type);
    void Hide();
    void ShowDialog(Sprite * emotion, dialog_info dialog);

private:
    Sprite * background;
    Text * character_name;
    Text * character_message;
    Timer keyCooldown;

    bool key_pressed;
};