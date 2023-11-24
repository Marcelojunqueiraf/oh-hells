#include "Game.hpp"

#include <iostream>

Game *Game::instance = nullptr;

Game::Game(std::string title, int width, int height)
{

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0)
  {
    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }

  if (IMG_Init((IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF)) != (IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF))
  {
    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }

  if (Mix_Init((MIX_INIT_OGG | MIX_INIT_MP3)) != (MIX_INIT_OGG | MIX_INIT_MP3))
  {
    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }

  if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) != 0)
  {
    Mix_VolumeMusic(10);

    for (int i = 0; i < 32; i++)
    {
      Mix_Volume(i, 10);
    }

    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }
  auto window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
  if (window == nullptr)
  {
    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }
  this->window = std::shared_ptr<SDL_Window>(window, SDL_DestroyWindow);

  auto renderer = SDL_CreateRenderer(this->window.get(), -1, SDL_RENDERER_ACCELERATED);
  if (renderer == nullptr)
  {
    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }
  this->renderer = std::shared_ptr<SDL_Renderer>(renderer, SDL_DestroyRenderer);

  frameStart = SDL_GetTicks();
}

Game *Game::GetInstance()
{
  if (Game::instance == nullptr)
  {
    Game::instance = new Game("Oh hells", 1024, 600);
  }
  return Game::instance;
}

void Game::run()
{
  state = std::make_shared<State>();
  state->Start();

  bool running = true;
  while (running)
  {
    this->CalculateDeltaTime();
    state->Update(this->dt);
    state->Render();

    if (state->QuitRequested())
      running = false;
    SDL_Delay(33);
  }
}

Game::~Game()
{
  Mix_CloseAudio();
  Mix_Quit();
  IMG_Quit();
  SDL_Quit();
  Resources::ClearResources();
}

void Game::CalculateDeltaTime()
{
  dt = (SDL_GetTicks() - frameStart) / 1000.0;
  frameStart = SDL_GetTicks();
}

int Game::GetWidth()
{
  int w;
  SDL_GetWindowSize(this->window.get(), &w, nullptr);
  return w;
}

int Game::GetHeight()
{
  int h;
  SDL_GetWindowSize(this->window.get(), nullptr, &h);
  return h;
}

std::weak_ptr<SDL_Renderer> Game::GetRenderer()
{
  return renderer;
}

std::weak_ptr<State> Game::GetCurrentState()
{
  return state;
}