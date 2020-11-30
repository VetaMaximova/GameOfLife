///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef INCLUDE_RULES_H_
#define INCLUDE_RULES_H_
#include "cell.h"

#include <cstdint>

///
/// @brief The CellBordersRule describes how do we deal with border cells
/// RingBorders in case you've got -1 coordinate for cell, you go to the right
/// border (ring borders) LimitedBorders no ring borders
///
enum class CellBordersRule { RingBorders, LimitedBorders };

///
/// @brief The GameRules return a new state of the cell according to rules,
/// cell index and the status of game
///
class GameRules {
public:
  /// @brief Get cell state in next generation
  ///
  /// @param input cell
  ///
  /// @return returns true if cell would be alive, otherwise false
  virtual bool GetNewCellState(const Cell &cell) const = 0;
  /// @brief Get index of cell in the world
  ///
  /// @param current_index index which we are interested in, max_index border
  /// value(maximum of rows/columns)
  ///
  /// @return set index according to border rules
  virtual void GetCellIndex(std::int32_t &current_index,
                            const std::uint32_t &max_index) const = 0;
  /// @brief Get the status of game (is it over)
  ///
  /// @param alive_cells_count count of alive cells in generation,
  /// equal_worlds_count count of generations which are equal to current,
  /// generations_count current generations count
  ///
  /// @return true if game is over, false otherwise
  virtual bool IsGameOver(const std::uint64_t &alive_cells_count,
                          const std::uint32_t &equal_worlds_count,
                          const std::uint32_t &generations_count) const = 0;
};

#endif // INCLUDE_RULES_H_
