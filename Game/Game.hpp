#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "../SDL_include.h"
#include "../States/State/State.hpp"
#include "../Utils/InputManager/InputManager.hpp"
#include <memory>
#include <stack>
#include <string>

#define GAME_WIDTH 1024
#define GAME_HEIGHT 600

class State;

class Game
{
private:
  static Game *instance;
  State * storedState;
  std::shared_ptr<SDL_Renderer> renderer;
  std::shared_ptr<SDL_Window> window;
  std::stack<std::unique_ptr<State>> stateStack;
  bool running;
  int frameStart;
  float dt;
  void CalculateDeltaTime();
  Game(std::string title, int width, int height);



public:
  static Game *GetInstance();
  void Run();
  void Push(State *state);
  std::weak_ptr<SDL_Renderer> GetRenderer();
  int GetWidth();
  int GetHeight();
  ~Game();
  State& GetCurrentState();
};
