#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Preguica/Preguica.hpp"
#include "../../Componentes/Player/Player.hpp"

class PreguicaState : public State {
public:
    PreguicaState ();
    ~PreguicaState();

    void LoadAssets () override;
    void Update (float dt) override;
    void Render () override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Music backgroundMusic;

};