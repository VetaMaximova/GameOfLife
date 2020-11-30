///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_WORLD_H_
#define INCLUDE_WORLD_H_
#include "cell.h"
#include "initial_figures/initial_figure.h"
#include "rules/rules.h"
#include "world_hasher.h"

#include <atomic>
#include <memory>
#include <vector>

using WorldCells = std::vector<std::vector<Cell>>;

///
/// @brief The World store all world's cells and has methods to
/// change cells state
///
class World {
public:
  /// @brief World is initialized with rows and columns count.
  /// The infinity of the world could be achieved by boundary rules
  World(const std::uint32_t rows, const std::uint32_t columns);
  /// @brief cell at row and column is marked as alive, cell neighbours are
  /// updated to increase number of alive cells
  void MakeCellAlive(const std::uint32_t row, const std::uint32_t column,
                     const GameRules &rules);
  /// @brief cell at row and column is marked as alive, cell neighbours are
  /// updated to decrease number of alive cells
  void MakeCellDied(const std::uint32_t row, const std::uint32_t column,
                    const GameRules &rules);
  /// @brief return rows of the world
  std::uint32_t GetRowCount();
  /// @brief return columns of the world
  std::uint32_t GetColumnCount();
  /// @brief return constant reference to cells
  const WorldCells &GetCells();
  /// @brief return constant reference to a specific cell
  const Cell &GetCellAt(const std::uint32_t row,
                        const std::uint32_t column) const;
  /// @brief set initial state of the world
  void SetInitialCells(const std::vector<Point> &alive_cells,
                       const GameRules &rules);
  /// @brief return count of alive cells
  std::uint64_t GetAliveCellsCount() const;
  /// @brief add hash of current world to set of hash results
  void UpdateHash();
  /// @brief return number of worlds with the same hashes
  std::uint32_t GetEqualWorldsCount();

private:
  /// @brief update neighbours of current cell in case state of the cell was
  /// changed
  void SetCellNeighbours(const std::uint32_t row, const std::uint32_t column,
                         const GameRules &rules);

  /// @brief cells of the world
  WorldCells cells;
  /// @brief hasher calculate current hash and stores previous hashes
  WorldHasher hasher;
  /// @brief stores count of alive cells
  std::atomic<std::uint64_t> alive_cells_count;

  /// @brief constants for rows and columns count
  const std::uint32_t cRowsCount, cColumnsCount;
};

#endif // INCLUDE_WORLD_H_
