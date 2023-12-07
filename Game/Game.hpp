#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#define INCLUDE_SDL_TTF
#include "../SDL_include.h"
#include "../States/State/State.hpp"
#include "../Music/Music.hpp"
#include "../Utils/InputManager/InputManager.hpp"
#include "../Componentes/Sprite/Sprite.hpp"
#include <memory>
#include <stack>
#include <string>

#define GAME_WIDTH 1024
#define GAME_HEIGHT 600

class State;
class Sprite;

class Game
{
private:
  static Game *instance;
  State *storedState;
  std::shared_ptr<SDL_Renderer> renderer;
  std::shared_ptr<SDL_Window> window;
  std::stack<std::unique_ptr<State>> stateStack;
  bool running;
  int frameStart;
  float dt;
  void CalculateDeltaTime();
  Game(std::string title, int width, int height);

public:
  static Sprite *dialogImage, *dialogBackground;
  static std::string character_name;
  static std::string character_msg;
  static bool show_dialog;

  static void SetDialog(std::string image, std::string chr_name, std::string chr_msg);
  static void SetDialog(std::string chr_name, std::string chr_msg);
  static void ShowDialog(bool show);

  Music backgroundMusic;
  static Game *GetInstance();
  void Run();
  void Push(State *state);
  std::weak_ptr<SDL_Renderer> GetRenderer();
  int GetWidth();
  int GetHeight();
  ~Game();
  State &GetCurrentState();
};
