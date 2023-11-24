#include "Component.hpp"
#include <iostream>

Component::Component(std::weak_ptr<GameObject> associated)
{
  this->associated = associated;
}

Component::~Component()
{
}

void Component::Update(float dt)
{
}

void Component::Render()
{
}

void Component::Start()
{
}

void Component::NotifyCollision(std::weak_ptr<GameObject> other)
{
}