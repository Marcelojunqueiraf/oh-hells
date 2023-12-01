#pragma once
#include <vector>
#include <string>
#include <memory>
#include "../Componentes/Component/Component.hpp"
#include "../Utils/Rect/Rect.hpp"


enum DepthMode{
  Normal,
  Dynamic,
  Top
};

class Component;

class GameObject
{
private:
  bool isDead;
  bool started;
  std::vector<std::shared_ptr<Component>> components;

public:
  DepthMode Depth = Normal;
  Rect box;
  float angle;
  GameObject();
  ~GameObject();
  void Start();
  void Update(float dt);
  void Render();
  bool IsDead();
  void RequestDelete();
  void AddComponent(Component *cpt);
  void RemoveComponent(Component *cpt);
  std::weak_ptr<Component> GetComponent(std::string type);
  void NotifyCollision(std::weak_ptr<GameObject> other);
};