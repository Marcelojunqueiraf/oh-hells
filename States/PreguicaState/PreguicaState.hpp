#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Dialog/Dialog.hpp"
#include "../../Componentes/Preguica/Preguica.hpp"
#include "../../Componentes/Player/Player.hpp"

class PreguicaState : public State
{
public:
    PreguicaState();
    ~PreguicaState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Rect game_view;
    Music backgroundMusic;

    Player *player;
    std::weak_ptr<GameObject> player_goPtr;

    bool dialog_finished = false;
    float dialog_time = 3;
    size_t dialog_index = 0;
    Timer dialogCooldown;

    Dialog *preguica_dialog;
    Sprite *preguica_dialog_animation;
};