#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Dialog/Dialog.hpp"
#include "../../Componentes/Luxuria/Luxuria.hpp"
#include "../../Componentes/Player/Player.hpp"

class LuxuriaState : public State
{
public:
    LuxuriaState();
    ~LuxuriaState();

    void LoadAssets() override;
    void Update(float dt) override;
    void Render() override;

    void Start() override;
    void Pause() override;
    void Resume() override;

private:
    Dialog *luxuria_dialog;

    Luxuria *luxuria;
    Sprite *luxuria_dialog_animation;

    Rect game_view;
    Player *player;

    Music backgroundMusic;

    std::weak_ptr<GameObject> player_goPtr;
};