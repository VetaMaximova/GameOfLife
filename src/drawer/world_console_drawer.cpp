///
/// @file
/// @copyright Copyright (C) 2020
///
#include "drawer/world_console_drawer.h"

#include <iostream>

void WorldConsoleDrawer::DrawHeadingLine(const std::uint32_t heading_size) {
  for (std::uint32_t column = 0; column < heading_size; ++column) {
    std::cout << "- ";
  }
  std::cout << std::endl;
}

void WorldConsoleDrawer::DrawCells(const WorldCells &cells) {
  if (cells.size() < 1 || cells[0].size() < 1) {
    std::cerr << "Rows and columns count should be at least 1" << std::endl;
    return;
  }

  DrawHeadingLine(cells[0].size());

  for (std::uint32_t row = 0; row < cells.size(); ++row) {
    for (std::uint32_t column = 0; column < cells[0].size(); ++column) {
      if (cells[row].size() < (column + 1)) {
        std::cerr << "Rows should have equal number of columns" << std::endl;
        return;
      }

      if (cells[row][column].IsAlive()) {
        std::cout << cRedColor << "X";
      } else {
        std::cout << cGreenColor << "-";
      }
      std::cout << " ";
    }
    std::cout << cNoColor << std::endl;
  }
}
