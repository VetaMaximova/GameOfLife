///
/// @file
/// @copyright Copyright (C) 2020
///
#include "world.h"

#include <iostream>

World::World(const std::uint32_t rows, const std::uint32_t columns)
    : cRowsCount(rows), cColumnsCount(columns), hasher(rows, columns) {
  for (std::uint32_t row = 0; row < rows; row++) {
    cells.push_back(std::vector<Cell>(columns));
  }

  alive_cells_count = 0;
}

void World::SetInitialCells(const std::vector<Point> &alive_cells,
                            const GameRules &rules) {
  for (auto &alive_cell : alive_cells) {
    MakeCellAlive(alive_cell.x, alive_cell.y, rules);
  }
}

std::uint32_t World::GetRowCount() { return cRowsCount; }

std::uint32_t World::GetColumnCount() { return cColumnsCount; }

const WorldCells &World::GetCells() { return cells; }

const Cell &World::GetCellAt(const std::uint32_t row,
                             const std::uint32_t column) const {
  if (row >= cRowsCount || column >= cColumnsCount) {
    std::cerr << "Incorrect column or row, returning last cell!" << std::endl;
    return cells[(row >= cRowsCount) ? (cRowsCount - 1) : row]
                [(column >= cColumnsCount) ? (cColumnsCount - 1) : column];
  }
  return cells[row][column];
}

void World::MakeCellAlive(const std::uint32_t row, const std::uint32_t column,
                          const GameRules &rules) {
  if (row >= cRowsCount || column >= cColumnsCount) {
    std::cerr << "Incorrect column or row" << std::endl;
    return;
  }

  if (cells[row][column].IsAlive()) {
    return;
  }
  cells[row][column].MakeAlive();
  alive_cells_count++;
  hasher.UpdateCellAlive(row, column);
  SetCellNeighbours(row, column, rules);
}

void World::MakeCellDied(const std::uint32_t row, const std::uint32_t column,
                         const GameRules &rules) {
  if (row >= cRowsCount || column >= cColumnsCount) {
    std::cerr << "Incorrect column or row" << std::endl;
    return;
  }

  if (!cells[row][column].IsAlive()) {
    return;
  }

  cells[row][column].MakeDied();
  alive_cells_count--;
  hasher.UpdateCellDied(row, column);
  SetCellNeighbours(row, column, rules);
}

void World::SetCellNeighbours(const std::uint32_t row,
                              const std::uint32_t column,
                              const GameRules &rules) {
  for (std::int32_t neighbour_row = static_cast<std::int32_t>(row) - 1;
       neighbour_row <= static_cast<std::int32_t>(row) + 1; ++neighbour_row) {
    for (std::int32_t neighbour_column = static_cast<std::int32_t>(column) - 1;
         neighbour_column <= static_cast<std::int32_t>(column) + 1;
         ++neighbour_column) {
      std::int32_t current_row = neighbour_row;
      rules.GetCellIndex(current_row, cRowsCount);

      std::int32_t current_column = neighbour_column;
      rules.GetCellIndex(current_column, cColumnsCount);

      if (current_row != row or current_column != column) {
        if (cells[row][column].IsAlive()) {
          cells[current_row][current_column].AddNeighbour();
        } else {
          cells[current_row][current_column].RemoveNeighbour();
        }
      }
    }
  }
}

void World::UpdateHash() { hasher.UpdateHash(); }

std::uint32_t World::GetEqualWorldsCount() { return hasher.EqualHashCount(); }

std::uint64_t World::GetAliveCellsCount() const { return alive_cells_count; }
