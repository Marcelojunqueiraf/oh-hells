#include "./Camera.hpp"

void Camera::Follow(GameObject *newFocus)
{
  focus = newFocus;
}

void Camera::Unfollow()
{
  focus = nullptr;
}

void Camera::Update(float dt)
{
  if (focus != nullptr)
  {
    this->pos = focus->box.GetCenter() - Vec2(Game::GetInstance()->GetWidth() / 2, Game::GetInstance()->GetHeight() / 2);
  }
  else
  {
    InputManager &input = InputManager::GetInstance();
    Vec2 movement = Vec2(0, 0);
    if (input.IsKeyDown(SDLK_w))
      movement.y -= 1;
    if (input.IsKeyDown(SDLK_s))
      movement.y += 1;
    if (input.IsKeyDown(SDLK_a))
      movement.x -= 1;
    if (input.IsKeyDown(SDLK_d))
      movement.x += 1;

    movement = movement.normalize();
    movement = movement * dt;
    this->pos = this->pos + Vec2(movement.x * this->speed.x, movement.y * this->speed.y);
  }
}

Camera &Camera::GetInstance()
{
  static Camera camera;
  return camera;
}

Camera::Camera()
{
  this->speed = Vec2(200, 200);
  this->pos = Vec2(0, 0);
  this->focus = nullptr;
}