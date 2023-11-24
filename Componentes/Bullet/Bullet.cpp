#include "./Bullet.hpp"

Bullet::Bullet(std::weak_ptr<GameObject> associated, float angle, float speed, int damage, float maxDistance, std::string sprite, bool targetPlayer) : Component(associated)
{
  associated.lock()->angle = angle;
  this->speed = Vec2(speed, 0).rotate(angle);
  this->distanceLeft = maxDistance;
  this->damage = damage;
  this->targetPlayer = targetPlayer;

  Sprite *spriteComponent = new Sprite(sprite, associated, 4, 0.2);
  this->associated.lock()->AddComponent(spriteComponent);
  this->associated.lock()->AddComponent(new Collider(associated, {1, 2}));
};

void Bullet::Update(float dt)
{
  this->distanceLeft -= this->speed.magnitude() * dt;
  if (this->distanceLeft <= 0)
  {
    this->associated.lock()->RequestDelete();
  }
  this->associated.lock()->box.x += this->speed.x * dt;
  this->associated.lock()->box.y += this->speed.y * dt;
}

bool Bullet::Is(std::string type)
{
  return type == "Bullet";
}

int Bullet::GetDamage()
{
  return this->damage;
}

void Bullet::NotifyCollision(std::weak_ptr<GameObject> other)
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