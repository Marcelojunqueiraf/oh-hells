#pragma once

#include <memory>
#include "../../GameObject/GameObject.hpp"
#include <string>
#include <iostream>
#include "../../Utils/Vec2/Vec2.hpp"
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Collider/Collider.hpp"

class Bullet : public Component
{
public:
  Bullet(std::weak_ptr<GameObject> associated, float angle, float speed, int damage, float maxDistance, std::string sprite, bool targetPlayer = false);
  void Update(float dt);
  bool Is(std::string type);
  int GetDamage();
  void NotifyCollision(std::weak_ptr<GameObject> other);
  bool targetPlayer;

private:
  Vec2 speed;
  float distanceLeft;
  int damage;
};