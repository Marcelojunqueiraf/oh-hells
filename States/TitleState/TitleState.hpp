#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Text/Text.hpp"
#include "../../Utils/Timer/Timer.hpp"

#define MENU_SIZE 2

class TitleState : public State {
public:
    bool key_pressed = false;
    Text * txt_start;  
    Timer key_delay;
    TitleState();
    ~TitleState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;
};