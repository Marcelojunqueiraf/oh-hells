#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Preguica/Preguica.hpp"
#include "../../Componentes/Player/Player.hpp"

class PreguicaState_2 : public State
{
public:
    PreguicaState_2();
    ~PreguicaState_2();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Rect game_view;

    Player *player;
    std::weak_ptr<GameObject> player_goPtr;
};