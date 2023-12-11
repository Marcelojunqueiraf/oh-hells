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

class GuidedBullet : public Bullet
{
public:
  GuidedBullet(std::weak_ptr<GameObject> associated, std::weak_ptr<GameObject> target, float speed, int damage, float maxDistance, std::string sprite, bool targetPlayer = false);
  void Update(float dt);
  void NotifyCollision(std::weak_ptr<GameObject> other);

private:
  Vec2 speed;
  float distanceLeft;
  int damage;
  std::weak_ptr<GameObject> target;
};