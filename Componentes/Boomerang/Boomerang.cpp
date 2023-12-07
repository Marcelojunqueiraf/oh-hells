#include "./Boomerang.hpp"

Boomerang::Boomerang(std::weak_ptr<GameObject> associated, Vec2 target, float speed, int damage, std::string sprite, std::weak_ptr<GameObject> origin, bool targetPlayer) : Bullet(associated, damage, sprite, targetPlayer)
{
  this->target = target;
  this->speed = speed;
  this->state = INDO;
  this->origin = origin;
};

void Boomerang::Update(float dt)
{
  Vec2 distance;
  if (state == INDO)
  {

    distance = this->target - this->associated.lock()->box.GetCenter();

    if (distance.magnitude() <= speed * dt)
    {
      state = VOLTANDO;
    }
  }
  else
  {
    if(!origin.expired()){
      distance = this->origin.lock()->box.GetCenter() - this->associated.lock()->box.GetCenter();
      if ((origin.lock()->box.GetCenter() - this->associated.lock()->box.GetCenter()).magnitude() <= speed * dt)
      {
        this->associated.lock()->RequestDelete();
      }
    }
    else{
      this->associated.lock()->RequestDelete();
    }
  }

  Vec2 velocity = distance.normalize() * this->speed;
  this->associated.lock()->box.x += velocity.x * dt;
  this->associated.lock()->box.y += velocity.y * dt;
}
