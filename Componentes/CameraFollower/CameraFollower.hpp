#pragma once
#include "../Component/Component.hpp"
#include "../../Camera/Camera.hpp"
#include <string>
#include <iostream>

class CameraFollower : public Component
{
public:
  CameraFollower(std::weak_ptr<GameObject> associated);
  void Start();
  void Update(float dt);
  void Render();
  bool Is(std::string type);
};