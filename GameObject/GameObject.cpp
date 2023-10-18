#include "GameObject.hpp"
#include <iostream>

GameObject::GameObject()
{
  this->isDead = false;
  this->box = Rect();
  this->angle = 0;
}

GameObject::~GameObject()
{
  this->components.clear();
}

void GameObject::Update(float dt)
{
  if (this->IsDead())
  {
    this->components.clear();
  }

  for (auto component : this->components)
  {
    component.get()->Update(dt);
  }
}

void GameObject::Render()
{
  for (auto component : this->components)
  {
    component.get()->Render();
  }
}

bool GameObject::IsDead()
{
  return this->isDead;
}

void GameObject::RequestDelete()
{
  this->isDead = true;
}

void GameObject::AddComponent(Component *cpt)
{
  this->components.emplace_back(cpt);
  if (started)
    cpt->Start();
}

void GameObject::RemoveComponent(Component *cpt)
{
  for (auto component : this->components)
  {
    if (component.get() == cpt)
    {
      this->components.erase(this->components.begin() + (component.get() - this->components[0].get()));
    }
  }
}

std::weak_ptr<Component> GameObject::GetComponent(std::string type)
{
  for (int i = 0; i < this->components.size(); i++)
  {
    if (this->components[i]->Is(type))
    {
      return this->components[i];
    }
  }
  return std::weak_ptr<Component>();
}

void GameObject::Start()
{
  for (auto component : this->components)
  {
    component.get()->Start();
  }
  started = true;
}