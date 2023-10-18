#include "./CameraFollower.hpp"

CameraFollower::CameraFollower(std::weak_ptr<GameObject> associated) : Component(associated)
{
}

void CameraFollower::Update(float dt)
{
  this->associated.lock()->box.x = Camera::GetInstance().pos.x;
  this->associated.lock()->box.y = Camera::GetInstance().pos.y;
}

void CameraFollower::Render()
{
}

bool CameraFollower::Is(std::string type)
{
  return type == "CameraFollower";
}

void CameraFollower::Start()
{
}