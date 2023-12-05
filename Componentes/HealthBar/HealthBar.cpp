#include "HealthBar.hpp"
#include "../../Camera/Camera.hpp"

HealthBar::HealthBar(std::string file, std::weak_ptr<GameObject> associated, int &hp, int maxHp) : Component(associated), hp(hp), maxHp(maxHp)
{
}

HealthBar::~HealthBar()
{
}

void HealthBar::Render()
{
  int barWidth = 100;
  float x = associated.lock()->box.GetCenter().x - Camera::GetInstance().pos.x - barWidth / 2;
  float y = associated.lock()->box.GetCenter().y - Camera::GetInstance().pos.y + associated.lock()->box.h / 4;

  // barra cinza atrás
  SDL_SetRenderDrawColor(Game::GetInstance()->GetRenderer().lock().get(), 50, 50, 50, SDL_ALPHA_OPAQUE);
  SDL_Rect clipRect = {(int)x, (int)y, barWidth, 10};
  SDL_RenderFillRect(Game::GetInstance()->GetRenderer().lock().get(), &clipRect);

  // barra vermelha na frente
  SDL_SetRenderDrawColor(Game::GetInstance()->GetRenderer().lock().get(), 252, 113, 109, SDL_ALPHA_OPAQUE);
  clipRect = {(int)x, (int)y, (int)(barWidth * ((float)hp / (float)maxHp)), 10};
  SDL_RenderFillRect(Game::GetInstance()->GetRenderer().lock().get(), &clipRect);
}

void HealthBar::Update(float dt)
{
}

bool HealthBar::Is(std::string type)
{
  return type == "HealthBar";
}

void HealthBar::Start()
{
  initial_hp = hp;
}