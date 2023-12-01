#pragma once

#include "../State/State.hpp"
#include "../../Componentes/Luxuria/Luxuria.hpp"
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

    Luxuria * luxuria;
    Sprite * luxuria_health_bar;
    std::weak_ptr<GameObject> luxuria_goPtr;


    Player * player;
    Sprite * player_health_bar;
    std::weak_ptr<GameObject> player_goPtr;

};