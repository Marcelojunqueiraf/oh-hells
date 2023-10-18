#pragma once

#include "../../GameObject/GameObject.hpp"
#include <iostream>

#define INCLUDE_SDL_MIXER
#include "../../SDL_include.h"

#include "../../Utils/Resources/Resources.hpp"

class Sound : public Component
{
private:
  Mix_Chunk *chunk;
  int channel;

public:
  Sound(std::weak_ptr<GameObject> associated);
  Sound(std::string file, std::weak_ptr<GameObject> associated);
  ~Sound();
  void Start();
  void Play(int times = 1);
  void Stop();
  void Open(std::string file);
  bool IsOpen();

  void Update(float dt);
  void Render();
  bool Is(std::string type);
};
