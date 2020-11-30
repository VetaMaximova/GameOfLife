///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "game_of_life.h"

#include <iostream>

int main(int argc, char **argv) {
  std::cout << "Game of life started" << std::endl;
  GameOfLife game(5, 10);
  game.FillInitialPicture(GameOfLifeInitialState::RandomLine);
  while (true) {
    game.Draw();
    game.ExecuteNextGeneration();
    if (game.IsGameOver()) {
      std::cout << "Game over" << std::endl;
      game.Draw();
      break;
    }
  }
}
