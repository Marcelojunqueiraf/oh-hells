#include "GuidedBullet.hpp"

GuidedBullet::GuidedBullet(std::weak_ptr<GameObject> associated, std::weak_ptr<GameObject> target, float speed, int damage, float maxDistance, std::string sprite, bool targetPlayer)
 : Bullet(associated, damage, sprite, targetPlayer), target(target)
{
  auto target_ptr = target.lock();
  auto assoc_ptr = this->associated.lock();

  Vec2 direction = target_ptr->box.GetCenter() - assoc_ptr->box.GetCenter();

  float angle = direction.getAngle();
  this->speed = Vec2(speed, 0).rotate(angle);
  this->distanceLeft = maxDistance;
  associated.lock()->angle = angle;
};

void GuidedBullet::Update(float dt)
{
    if(!target.expired()){
        auto target_ptr = target.lock();
        auto assoc_ptr = this->associated.lock();

        Vec2 direction = target_ptr->box.GetCenter() - assoc_ptr->box.GetCenter();

        this->distanceLeft -= this->speed.magnitude() * dt;
        if (this->distanceLeft <= 0)
        {
            this->associated.lock()->RequestDelete();
        }

        
        Vec2 velocity = direction.normalize() * this->speed.magnitude();

        assoc_ptr->angle = velocity.getAngle();
        assoc_ptr->box.x += velocity.x * dt;
        assoc_ptr->box.y += velocity.y * dt;
    }
    else{
        associated.lock()->RequestDelete();
    }
  
}

void GuidedBullet::NotifyCollision(std::weak_ptr<GameObject> other)
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