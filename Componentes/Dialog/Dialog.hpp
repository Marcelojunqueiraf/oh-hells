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
#include <string>

struct dialog_info{
    float time;
    std::string character_name;
    std::string character_msg;
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
    void ShowDialog(Sprite * emotion, std::string chr_name, std::string chr_msg, float timeout=5);

private:
    Sprite * background;
    Sprite *last_character_animation = nullptr;
    Text * character_name;
    Text * character_message;
    Sound * dialogSound;

    bool dialog_showing1 = false;
    float dialog_timeout1;
    Timer dialog_timer1;
};