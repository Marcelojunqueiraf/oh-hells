#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "../../SDL_include.h"
#include "../../Game/Game.hpp"
#include "../../GameObject/GameObject.hpp"
#include "../../Utils/Resources/Resources.hpp"


#include <iostream>

class HealthBar : public Component
{
private:
  Sprite * health_bar;
  int& hp;
  int initial_hp;

public:
  HealthBar(std::string file, std::weak_ptr<GameObject> associated, int& hp);
  ~HealthBar();
  void Start();

  void Update(float dt);
  void Render();
  bool Is(std::string type);

  bool show = true;
};