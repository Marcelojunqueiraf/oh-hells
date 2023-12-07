#include "Game.hpp"

#include <iostream>
#define DEBUG

Game *Game::instance = nullptr;
Sprite *Game::dialogImage = nullptr;
Sprite *Game::dialogBackground = nullptr;
std::string Game::character_name = "";
std::string Game::character_msg = "";
bool Game::show_dialog = false;

TTF_Font *Game::Sans;
SDL_Texture * Game::dialog_Message1;
SDL_Texture * Game::dialog_Message2;
SDL_Rect Game::Message1_rect = {395, 380, 0, 0};;
SDL_Rect Game::Message2_rect = {400, 430, 0, 0};


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
  Sans = Resources::GetFont("Assets/Ubuntu-Regular.ttf", 28);

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

        SDL_RenderCopy(renderer.get(), dialog_Message1, NULL, &Message1_rect);
        SDL_RenderCopy(renderer.get(), dialog_Message2, NULL, &Message2_rect);
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

  if(Game::dialog_Message1)

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

  Game::SetDialog(std::move(chr_name), std::move(chr_msg));
}

void Game::SetDialog(std::string chr_name, std::string chr_msg)
{
  SDL_Color White = {255, 255, 255};
  SDL_Surface *surf = TTF_RenderUTF8_Blended_Wrapped(Sans, chr_msg.c_str(), White, 500);

  if (dialog_Message2)
      SDL_DestroyTexture(dialog_Message2);
    
	dialog_Message2 = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer().lock().get(), surf);
  Game::character_msg = chr_msg;
  Message2_rect.w = surf->w;
  Message2_rect.h = surf->h;
  SDL_FreeSurface(surf);

  if(chr_msg != Game::character_name){
    surf = TTF_RenderUTF8_Solid(Sans, chr_name.c_str(), White);

    if (dialog_Message1)
      SDL_DestroyTexture(dialog_Message1);
    
    dialog_Message1 = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer().lock().get(), surf);
    Game::character_name = chr_name;

    Message1_rect.w = surf->w;
    Message1_rect.h = surf->h;
	  SDL_FreeSurface(surf);
  }
}

void Game::ShowDialog(bool show)
{
  Game::show_dialog = show;
}