#include "./Camera.hpp"

void Camera::Follow(GameObject *newFocus)
{
  focus = newFocus;
}

void Camera::Unfollow()
{
  focus = nullptr;
}

void Camera::SetView(Rect max_view)
{
  this->max_view = max_view;
}

void Camera::Update(float dt)
{
  if (focus != nullptr)
  {
    this->pos = focus->box.GetCenter() - Vec2(Game::GetInstance()->GetWidth() / 2, Game::GetInstance()->GetHeight() / 2);
    pos.x = (pos.x < max_view.x) ? max_view.x : pos.x;
    pos.x = (pos.x > max_view.x+max_view.w-GAME_WIDTH) ? max_view.x+max_view.w-GAME_WIDTH: pos.x;
    pos.y = (pos.y < max_view.y) ? max_view.y : pos.y;
    pos.y = (pos.y > max_view.x+max_view.h-GAME_HEIGHT) ? max_view.x+max_view.h-GAME_HEIGHT: pos.y;
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
  this->max_view= {-GAME_WIDTH,-GAME_HEIGHT,GAME_WIDTH, GAME_HEIGHT};
}