// #pragma once

// #include "../../Component/Component.hpp"
// #include "../../Sprite/Sprite.hpp"
// #include "../../../Game/Game.hpp"
// #include "../../../Utils/InputManager/InputManager.hpp"
// #include "../../../Camera/Camera.hpp"
// #include "../../Bullet/Bullet.hpp"
// #include "../../../Utils/Timer/Timer.hpp"
// #include "../../Enemy/Enemy.hpp"

// #include <memory>

// class Sprite;
// class Enemy;

// class Minion : public Component
// {

// public:
//   Minion(std::weak_ptr<GameObject> associated);
//   ~Minion();
//   void Update(float dt);
//   bool Is(std::string type);
//   void NotifyCollision(std::weak_ptr<GameObject> other);

// private:
//   Enemy *enemy;
//   Timer shootCooldown;
//   Timer hitTimer;
//   Sprite *idle_animation, *hit_animation;
//   Sprite *last_animation;
// };