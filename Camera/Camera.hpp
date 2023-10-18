#pragma once

#include "../Utils/Vec2/Vec2.hpp"
#include "../GameObject/GameObject.hpp"
#include "../Game/Game.hpp"
#include "../Utils/InputManager/InputManager.hpp"

class Camera
{
private:
  GameObject *focus;

public:
  Vec2 speed;
  Vec2 pos;
  void Follow(GameObject *newFocus);
  void Unfollow();
  void Update(float dt);
  static Camera &GetInstance();
  Camera();
};