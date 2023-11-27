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
    Game::instance = new Game("Oh hells", GAME_WIDTH, GAME_HEIGHT);
  }
  return Game::instance;
}

void Game::Run()
{
  if (storedState != nullptr) {
		stateStack.emplace(storedState);
		storedState = nullptr;
		stateStack.top()->Start();
		while (!stateStack.top()->QuitRequested() && !stateStack.empty()) {
			if (stateStack.top()->PopRequested()) {
				stateStack.pop();
				Resources::ClearImages();
				Resources::ClearMusics();
				Resources::ClearSounds();
				// Resources::ClearFonts();
				if (!stateStack.empty()) {
					stateStack.top()->Resume();
				}
			}

			if (storedState != nullptr) {
				if(!stateStack.empty()) stateStack.top()->Pause();
				stateStack.emplace(storedState);
				storedState = nullptr;
				stateStack.top()->Start();
			}

			if (stateStack.empty()) break;

			CalculateDeltaTime();
			InputManager::GetInstance().Update();
			stateStack.top()->Update(dt);
			stateStack.top()->Render();
			SDL_RenderPresent(renderer.get());
			SDL_Delay(33);
		}
	}

	while (!stateStack.empty()) {
		stateStack.pop();
	}

	Resources::ClearSounds();
	Resources::ClearMusics();
	Resources::ClearImages();
	// Resources::ClearFonts();
}

Game::~Game()
{
	// Deletar State
	if (storedState != nullptr) {
		delete storedState;
	}

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

void Game::Push(State *state) {
    storedState = state;
}

std::weak_ptr<SDL_Renderer> Game::GetRenderer()
{
  return renderer;
}

State& Game::GetCurrentState()
{
  return *(stateStack.top().get());
}