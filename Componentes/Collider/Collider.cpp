#define DEBUG

#ifdef DEBUG
#include "../../Camera/Camera.hpp"
#include "../../Game/Game.hpp"

#include <SDL2/SDL.h>
#endif // DEBUG

#include "Collider.hpp"

Collider::Collider(std::weak_ptr<GameObject> associated, Vec2 scale, Vec2 offset) : Component(associated)
{
  this->scale = scale;
  this->offset = offset;
}

void Collider::Update(float dt)
{
  if (auto associatedPointer = associated.lock())
  {
    // float widthDifference = associatedPointer->box.w * (scale.x - 1) / 2;
    // float heightDifference = associatedPointer->box.h * (scale.y - 1) / 2;
    this->box = Rect(associatedPointer->box.x + this->offset.x, associatedPointer->box.y + this->offset.y, associatedPointer->box.w * scale.x, associatedPointer->box.h * scale.y);
  }
  else
  {
    this->box = Rect(0, 0, 0, 0);
  }
}

bool Collider::Is(std::string type)
{
  return type == "Collider";
}

void Collider::SetScale(Vec2 scale)
{
  this->scale = scale;
}

void Collider::SetOffset(Vec2 offset)
{
  this->offset = offset;
}

void Collider::Render()
{
#ifdef DEBUG
  Vec2 center(box.GetCenter());
  SDL_Point points[5];

  std::shared_ptr<GameObject> associated = this->associated.lock();
  Camera camera = Camera::GetInstance();

  Vec2 point = (Vec2(box.x, box.y) - center).rotate(associated->angle) + center - camera.pos;
  points[0] = {(int)point.x, (int)point.y};
  points[4] = {(int)point.x, (int)point.y};

  point = (Vec2(box.x + box.w, box.y) - center).rotate(associated->angle) + center - camera.pos;
  points[1] = {(int)point.x, (int)point.y};

  point = (Vec2(box.x + box.w, box.y + box.h) - center).rotate(associated->angle) + center - camera.pos;
  points[2] = {(int)point.x, (int)point.y};

  point = (Vec2(box.x, box.y + box.h) - center).rotate(associated->angle) + center - camera.pos;
  points[3] = {(int)point.x, (int)point.y};

  SDL_SetRenderDrawColor(Game::GetInstance()->GetRenderer().lock().get(), 255, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderDrawLines(Game::GetInstance()->GetRenderer().lock().get(), points, 5);
#endif // DEBUG
}