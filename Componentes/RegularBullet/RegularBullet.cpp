#include "./RegularBullet.hpp"

RegularBullet::RegularBullet(std::weak_ptr<GameObject> associated, float angle, float speed, int damage, float maxDistance, std::string sprite, bool targetPlayer) : Bullet(associated, damage, sprite, targetPlayer)
{
  associated.lock()->angle = angle;
  this->speed = Vec2(speed, 0).rotate(angle);
  this->distanceLeft = maxDistance;
};

void RegularBullet::Update(float dt)
{
  this->distanceLeft -= this->speed.magnitude() * dt;
  if (this->distanceLeft <= 0)
  {
    this->associated.lock()->RequestDelete();
  }
  this->associated.lock()->box.x += this->speed.x * dt;
  this->associated.lock()->box.y += this->speed.y * dt;
}

void RegularBullet::NotifyCollision(std::weak_ptr<GameObject> other)
{
  std::shared_ptr<GameObject> otherPtr = other.lock();
  if (this->targetPlayer && otherPtr->GetComponent("Player").lock() != nullptr)
  {
    associated.lock()->RequestDelete();
  }
  else if (!this->targetPlayer && otherPtr->GetComponent("Enemy").lock() != nullptr)
  {
    associated.lock()->RequestDelete();
  }
}