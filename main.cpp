#include "Game/Game.hpp"
#include "States/TitleState/TitleState.hpp"

int main()
{
  Game *game = Game::GetInstance();
  game->Push(new TitleState());
  game->Run();
  return 0;
}
