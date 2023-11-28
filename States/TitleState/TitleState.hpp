#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Text/Text.hpp"
#include "../../Utils/Timer/Timer.hpp"

#define MENU_SIZE 2

class TitleState : public State {
public:
    int menu_option = 0;
    Text * txt_luxuria_option, * txt_preguica_option;  
    Timer key_delay;
    TitleState();
    ~TitleState();
    void StartLevel();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;
};