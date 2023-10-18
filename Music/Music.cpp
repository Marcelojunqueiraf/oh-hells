#include "Music.hpp"

Music::Music()
{
  music = nullptr;
}

Music::Music(std::string file)
{
  music = nullptr;
  Open(file);
}

Music::~Music()
{
  Stop();
}

void Music::Play(int times)
{
  if (music != nullptr)
  {
    Mix_PlayMusic(music, times);
  }
}

void Music::Stop(int msToStop)
{
  Mix_FadeOutMusic(msToStop);
}

void Music::Open(std::string file)
{
  if (music != nullptr)
  {
    Stop();
    Mix_FreeMusic(music);
  }
  music = Mix_LoadMUS(file.c_str());
  if (music == nullptr)
  {
    std::cerr << SDL_GetError() << std::endl;
    std::terminate();
  }
}

bool Music::IsOpen()
{
  return music != nullptr;
}
