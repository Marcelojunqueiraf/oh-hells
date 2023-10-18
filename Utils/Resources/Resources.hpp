#pragma once

#define INCLUDE_SDL_IMAGE
#define INCLUDE_SDL_MIXER

#include <unordered_map>
#include <iostream>
#include <string>
#include "../../SDL_include.h"
#include "../../Game/Game.hpp"

class Resources
{
private:
  static std::unordered_map<std::string, SDL_Texture *> imageTable;
  static std::unordered_map<std::string, Mix_Music *> musicTable;
  static std::unordered_map<std::string, Mix_Chunk *> audioTable;

public:
  static SDL_Texture *GetImage(std::string file);
  static void ClearImages();

  static Mix_Music *GetMusic(std::string file);
  static void ClearMusics();

  static Mix_Chunk *GetSound(std::string file);
  static void ClearSounds();

  static void ClearResources();
};