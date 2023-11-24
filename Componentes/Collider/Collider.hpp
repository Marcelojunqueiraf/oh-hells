#pragma once

#include <memory>
#include "../../Utils/Vec2/Vec2.hpp"
#include "../../Utils/Rect/Rect.hpp"
#include "../../GameObject/GameObject.hpp"
#include "../Component/Component.hpp"

class Collider : public Component
{
public:
  Collider(std::weak_ptr<GameObject> associated, Vec2 scale = {1, 1}, Vec2 offset = {0, 0});
  Rect box;
  void Update(float dt);
  bool Is(std::string type);
  void SetScale(Vec2 scale);
  void SetOffset(Vec2 offset);
  void Render();

private:
  Vec2 scale;
  Vec2 offset;
};