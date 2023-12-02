#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Preguica/Preguica.hpp"
#include "../../Componentes/Player/Player.hpp"

class PreguicaState_1 : public State {
public:
    PreguicaState_1();
    ~PreguicaState_1();

    void LoadAssets () override;
    void Update (float dt) override;
    void Render () override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Music backgroundMusic;

    Player * player;

};