#pragma once

#include <memory>
#include "../../GameObject/GameObject.hpp"
#include <string>
#include <iostream>
#include "../../Utils/Vec2/Vec2.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Collider/Collider.hpp"
#include "../Bullet/Bullet.hpp"

class Boomerang : public Bullet
{
public:
  Boomerang(std::weak_ptr<GameObject> associated, Vec2 target, float speed, int damage, std::string sprite, std::weak_ptr<GameObject> origin, bool targetPlayer = false);
  void Update(float dt);

private:
  enum State
  {
    INDO,
    VOLTANDO
  };
  State state;
  std::weak_ptr<GameObject> origin;
  Vec2 target;
  float speed;
};