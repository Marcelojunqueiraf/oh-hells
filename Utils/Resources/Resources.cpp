#include "Resources.hpp"

std::unordered_map<std::string, SDL_Texture *> Resources::imageTable;
std::unordered_map<std::string, Mix_Music *> Resources::musicTable;
std::unordered_map<std::string, Mix_Chunk *> Resources::audioTable;
std::unordered_map<std::string, TTF_Font *> Resources::fontTable;

SDL_Texture *Resources::GetImage(std::string file)
{
  if (Resources::imageTable.find(file) == Resources::imageTable.end())
  {
    SDL_Texture *texture = IMG_LoadTexture(Game::GetInstance()->GetRenderer().lock().get(), file.c_str());
    if (texture == nullptr)
    {
      std::cerr << SDL_GetError() << std::endl;
      std::terminate();
    }
    Resources::imageTable.emplace(file, texture);
  }
  return Resources::imageTable[file];
}

void Resources::ClearImages()
{
  for (auto &texture : Resources::imageTable)
  {
    SDL_DestroyTexture(texture.second);
  }
  Resources::imageTable.clear();
}

Mix_Music *Resources::GetMusic(std::string file)
{
  if (Resources::musicTable.find(file) == Resources::musicTable.end())
  {
    Mix_Music *music = Mix_LoadMUS(file.c_str());
    if (music == nullptr)
    {
      std::cerr << SDL_GetError() << std::endl;
      std::terminate();
    }
    Resources::musicTable.emplace(file, music);
  }
  return Resources::musicTable[file];
}

void Resources::ClearMusics()
{
  for (auto music : Resources::musicTable)
  {
    Mix_FreeMusic(music.second);
  }
  Resources::musicTable.clear();
}

Mix_Chunk *Resources::GetSound(std::string file)
{
  if (Resources::audioTable.find(file) == Resources::audioTable.end())
  {
    Mix_Chunk *sound = Mix_LoadWAV(file.c_str());
    if (sound == nullptr)
    {
      std::cerr << SDL_GetError() << std::endl;
      std::terminate();
    }
    Resources::audioTable.emplace(file, sound);
  }
  return Resources::audioTable[file];
}

void Resources::ClearSounds()
{
  for (auto sound : Resources::audioTable)
  {
    Mix_FreeChunk(sound.second);
  }
  Resources::audioTable.clear();
}

TTF_Font *Resources::GetFont(std::string file, int ptsize)
{
  auto specific_file = file + std::to_string(ptsize);
  if (Resources::fontTable.find(specific_file) == Resources::fontTable.end())
  {
    TTF_Font* font = TTF_OpenFont(file.c_str(), ptsize);
    if (font == nullptr)
    {
      std::cerr << SDL_GetError() << std::endl;
      std::terminate();
    }
    Resources::fontTable.emplace(specific_file, font);
  }
  return Resources::fontTable[specific_file];
}

void Resources::ClearFonts()
{
  for (auto &font : Resources::fontTable)
  {
    TTF_CloseFont(font.second);
  }
  Resources::fontTable.clear();
}


void Resources::ClearResources()
{
  ClearImages();
  ClearMusics();
  ClearSounds();
  ClearFonts();
}