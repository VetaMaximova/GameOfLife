///
/// @file
/// @copyright Copyright (C) 2020
///
#include "rules/conway_rules.h"

bool ConwayRules::GetNewCellState(const Cell &cell) const {
  if (cell.IsAlive()) {
    return cSurvivalCount.find(cell.GetAliveNeighboursCount()) !=
           cSurvivalCount.end();
  } else {
    return cRebirthCount.find(cell.GetAliveNeighboursCount()) !=
           cRebirthCount.end();
  }
}

void ConwayRules::GetCellIndex(std::int32_t &current_index,
                               const std::uint32_t &max_index) const {
  if (current_index < 0) {
    current_index += max_index;
  }
  current_index %= max_index;
}

bool ConwayRules::IsGameOver(const std::uint64_t &alive_cells_count,
                             const std::uint32_t &equal_worlds_count,
                             const std::uint32_t &generations_count) const {
  if (alive_cells_count < cMinAliveCellsCount ||
      equal_worlds_count > cMaxEqualWorldsCount ||
      generations_count > cMaxGenerations) {
    return true;
  }
  return false;
}
