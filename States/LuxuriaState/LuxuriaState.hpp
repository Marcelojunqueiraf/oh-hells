#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Enemy/Enemy.hpp"
#include "../../Componentes/Player/Player.hpp"

class LuxuriaState : public State {
public:
    LuxuriaState ();
    ~LuxuriaState();

    void LoadAssets () override;
    void Update (float dt) override;
    void Render () override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Music backgroundMusic;

};