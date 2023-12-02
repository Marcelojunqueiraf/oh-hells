#include "HealthBar.hpp"
#include "../../Camera/Camera.hpp"

HealthBar::HealthBar(std::string file, std::weak_ptr<GameObject> associated, int& hp) : 
Component(associated), hp(hp)
{
  health_bar = new Sprite(std::move(file), associated);
  health_bar->SetScaleX(3, 3);
}

HealthBar::~HealthBar()
{
}

void HealthBar::Render()
{
  health_bar->Render();
}

void HealthBar::Update(float dt)
{
  health_bar->SetClip(0,0, 20+hp/4, 64);
}

bool HealthBar::Is(std::string type)
{
  return type == "HealthBar";
}

void HealthBar::Start()
{
  initial_hp = hp;
}