#pragma once

#include <memory>
#include "../../GameObject/GameObject.hpp"
#include <string>
#include <iostream>
#include "../Component/Component.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Collider/Collider.hpp"

class Bullet : public Component
{
public:
  Bullet(std::weak_ptr<GameObject> associated, int damage, std::string sprite, bool targetPlayer = false);
  bool Is(std::string type);
  int GetDamage();
  bool targetPlayer;

private:
  int damage;
};