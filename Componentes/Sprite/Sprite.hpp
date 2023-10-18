#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "../../SDL_include.h"
#include "../../Game/Game.hpp"
#include "../../GameObject/GameObject.hpp"
#include "../../Utils/Resources/Resources.hpp"
#include <iostream>

class Sprite : public Component
{
private:
  SDL_Texture *texture;
  int width;
  int height;
  SDL_Rect clipRect;
  Vec2 scale;
  int frameCount;
  int currentFrame;
  float timeElapsed;
  float frameTime;

public:
  Sprite(std::weak_ptr<GameObject> associated);
  Sprite(std::string file, std::weak_ptr<GameObject> associated, int frameCount = 1, float frameTime = 1);
  ~Sprite();
  void Start();
  void Open(std::string file);
  void SetClip(int x, int y, int w, int h);

  void SetScaleX(float scaleX, float scaleY);
  Vec2 GetScale();

  void Render(int x, int y);
  int GetWidth();
  int GetHeight();
  bool IsOpen();

  void Update(float dt);
  void Render();
  bool Is(std::string type);

  void SetFrame(int frame);
  void SetFrameCount(int frameCount);
  void SetFrameTime(float frameTime);
};