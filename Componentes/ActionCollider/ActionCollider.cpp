#include "ActionCollider.hpp"

ActionCollider::ActionCollider(std::weak_ptr<GameObject> associated, Vec2 scale, Vec2 offset, State* state, void (*callbak)(State*, std::weak_ptr<GameObject>)) 
: Component(associated), state(state), callbak(callbak)
{
    auto assoc = associated.lock();
    assoc->AddComponent(new Collider(associated, scale, offset));
}

ActionCollider::~ActionCollider()
{
}

bool ActionCollider::Is(std::string type)
{
    return type == "ActionCollider";
}

void ActionCollider::NotifyCollision(std::weak_ptr<GameObject> other)
{
    callbak(state, other);
}
