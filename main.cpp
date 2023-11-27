#include "Game/Game.hpp"
#include "States/LuxuriaState/LuxuriaState.hpp"

int main()
{
  Game *game = Game::GetInstance();
  game->Push(new LuxuriaState());
  game->Run();
  return 0;
}
