#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "../SDL_include.h"
#include "../State/State.hpp"
#include <memory>
#include <string>

class State;

class Game
{
private:
  static Game *instance;
  std::shared_ptr<State> state;
  std::shared_ptr<SDL_Renderer> renderer;
  std::shared_ptr<SDL_Window> window;
  bool running;
  int frameStart;
  float dt;
  void CalculateDeltaTime();
  Game(std::string title, int width, int height);

public:
  static Game *GetInstance();
  void run();
  std::weak_ptr<SDL_Renderer> GetRenderer();
  int GetWidth();
  int GetHeight();
  ~Game();
  std::weak_ptr<State> GetCurrentState();
};
