#include "Game.hpp"

#include <iostream>
#define DEBUG

Game *Game::instance = nullptr;
Sprite *Game::dialogImage = nullptr;
Sprite *Game::dialogBackground = nullptr;
std::string Game::character_name = "";
std::string Game::character_msg = "";
bool Game::show_dialog = false;

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

  if (TTF_Init() != 0)
  {
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
  // Game::SetDialog("Assets/preguica_dialog.png", "Character", "Message");
  // Game::ShowDialog(true);
  Game::dialogBackground = new Sprite("Assets/Menu_dialogo.png", std::weak_ptr<GameObject>());
  Game::dialogBackground->SetScaleX((float)GAME_WIDTH / Game::dialogBackground->GetWidth(), (float)GAME_HEIGHT / Game::dialogBackground->GetHeight());

  if (storedState != nullptr)
  {
    stateStack.emplace(storedState);
    storedState = nullptr;
    stateStack.top()->Start();
    while (!stateStack.top()->QuitRequested() && !stateStack.empty())
    {
      if (stateStack.top()->PopRequested())
      {
        stateStack.pop();
        // Resources::ClearResources();
        if (!stateStack.empty())
        {
          stateStack.top()->Resume();
        }
      }

      if (storedState != nullptr)
      {
        if (!stateStack.empty())
          stateStack.top()->Pause();
        stateStack.emplace(storedState);
        storedState = nullptr;
        stateStack.top()->Start();
      }

      if (stateStack.empty())
        break;

      CalculateDeltaTime();
      InputManager::GetInstance().Update();
      stateStack.top()->Update(dt);
      stateStack.top()->Render();

      if (show_dialog && Game::show_dialog)
      {
        Vec2 pos = Camera::GetInstance().pos;
        Game::dialogBackground->Render(pos.x, pos.y);
        Game::dialogImage->Render(pos.x, pos.y);

        TTF_Font *Sans = TTF_OpenFont("Assets/Ubuntu-Regular.ttf", 28);

        SDL_Color White = {255, 255, 255};

        SDL_Surface *surfaceMessage =
            TTF_RenderText_Solid(Sans, Game::character_name.c_str(), White);

        SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer.get(), surfaceMessage);

        SDL_Rect Message_rect;              // create a rect
        Message_rect.x = 395;               // controls the rect's x coordinate
        Message_rect.y = 380;               // controls the rect's y coordinte
        Message_rect.w = surfaceMessage->w; // controls the width of the rect
        Message_rect.h = surfaceMessage->h; // controls the height of the rect

        SDL_RenderCopy(renderer.get(), Message, NULL, &Message_rect);

        surfaceMessage =
            TTF_RenderText_Solid(Sans, Game::character_msg.c_str(), White);

        Message = SDL_CreateTextureFromSurface(renderer.get(), surfaceMessage);

        Message_rect.x = 400;               // controls the rect's x coordinate
        Message_rect.y = 430;               // controls the rect's y coordinte
        Message_rect.w = surfaceMessage->w; // controls the width of the rect
        Message_rect.h = surfaceMessage->h; // controls the height of the rect

        SDL_RenderCopy(renderer.get(), Message, NULL, &Message_rect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
      }

      SDL_RenderPresent(renderer.get());
      SDL_Delay(33);
    }
  }

  while (!stateStack.empty())
  {
    stateStack.pop();
  }

  Resources::ClearResources();
}

Game::~Game()
{
  // Deletar State
  if (storedState != nullptr)
  {
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

void Game::Push(State *state)
{
  storedState = state;
}

std::weak_ptr<SDL_Renderer> Game::GetRenderer()
{
  return renderer;
}

State &Game::GetCurrentState()
{
  return *(stateStack.top().get());
}

void Game::SetDialog(std::string image, std::string chr_name, std::string chr_msg)
{
  if (Game::dialogImage != nullptr)
    free(Game::dialogImage);
  Game::dialogImage = new Sprite(image, std::weak_ptr<GameObject>());
  Game::dialogImage->SetScaleX((float)GAME_WIDTH / Game::dialogImage->GetWidth(), (float)GAME_HEIGHT / Game::dialogImage->GetHeight());
  Game::character_name = chr_name;
  Game::character_msg = chr_msg;
}

void Game::SetDialog(std::string chr_name, std::string chr_msg)
{
  Game::character_name = chr_name;
  Game::character_msg = chr_msg;
}

void Game::ShowDialog(bool show)
{
  Game::show_dialog = show;
}