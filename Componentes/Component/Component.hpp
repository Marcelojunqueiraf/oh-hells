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
  virtual void Update(float dt);
  virtual void Render();
  virtual void Start();
  virtual bool Is(std::string type) = 0;
  virtual void NotifyCollision(std::weak_ptr<GameObject> other);
};
