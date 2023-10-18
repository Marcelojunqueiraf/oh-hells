#pragma once
#include "../../GameObject/GameObject.hpp"
#include <memory>

class GameObject;

class Component
{
protected:
  std::weak_ptr<GameObject> associated;

public:
  Component(std::weak_ptr<GameObject> associated);
  virtual ~Component();
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;
  virtual void Start() = 0;
  virtual bool Is(std::string type) = 0;
};
