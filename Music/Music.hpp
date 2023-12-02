#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER
#include "../SDL_include.h"
#include <iostream>

class Music
{
private:
  Mix_Music *music;

public:
  Music();
  Music(std::string file);
  ~Music();
  void Play(int times = -1);
  void Stop(int msToStop = 1500);
  void Pause(){Mix_PauseMusic();}
  void Resume(){Mix_ResumeMusic();}
  void Open(std::string file);
  bool IsOpen();
};