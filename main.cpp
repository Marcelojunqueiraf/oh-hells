#include "Game/Game.hpp"

int main()
{
  Game *game = Game::GetInstance();
  game->run();
  return 0;
}
