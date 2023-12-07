#pragma once

#include "../../Game/Game.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Sound/Sound.hpp"
#include "../../Utils/InputManager/InputManager.hpp"
#include "../../Camera/Camera.hpp"
#include "../RegularBullet/RegularBullet.hpp"
#include "../../Utils/Timer/Timer.hpp"
#include "../../Componentes/Minion/Minion.hpp"

#include <memory>

class Sprite;

class Preguica : public Component
{

public:
    Preguica(std::weak_ptr<GameObject> associated, int hp, std::weak_ptr<GameObject> player_go);
    ~Preguica();
    void Update(float dt);
    bool Is(std::string type);
    void NotifyCollision(std::weak_ptr<GameObject> other);
    void ShowSprite(Sprite *spr);
    int &GetHp();
    void TakeDamage(int damage);

private:
    enum FASES
    {
        ESPERANDO,
        TIRO,
        MINION,
        CIRCULO,
        TORNADO,
        MORRENDO,
    };

    int hp;
    std::weak_ptr<GameObject> player_go;
    Timer shootCooldown, moveCooldown;
    Timer hitTimer;
    Timer dialogTimer;
    Sprite *idle_animation, *hit_animation;
    int currentPosition;

    float shootAngle;
    FASES fase;
};