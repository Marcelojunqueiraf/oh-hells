#include "./Bullet.hpp"

Bullet::Bullet(std::weak_ptr<GameObject> associated, int damage, std::string sprite, bool targetPlayer) : Component(associated)
{
  this->damage = damage;
  this->targetPlayer = targetPlayer;

  Sprite *spriteComponent = new Sprite(sprite, associated, 4, 0.1);
  spriteComponent->SetScaleX(2, 2);

  this->associated.lock()->AddComponent(spriteComponent);
  this->associated.lock()->AddComponent(new Collider(associated, {1, 1}, {2, 2}));
};

bool Bullet::Is(std::string type)
{
  return type == "Bullet";
}

int Bullet::GetDamage()
{
  return this->damage;
}
