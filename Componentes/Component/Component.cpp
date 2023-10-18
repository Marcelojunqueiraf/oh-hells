#include "Component.hpp"
#include <iostream>

Component::Component(std::weak_ptr<GameObject> associated)
{
  this->associated = associated;
}

Component::~Component()
{
}