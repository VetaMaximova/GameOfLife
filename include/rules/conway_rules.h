///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef INCLUDE_CONWAY_RULES_H_
#define INCLUDE_CONWAY_RULES_H_
#include "rules.h"

#include <set>

///
/// @brief The ConwayRules describes game rules suggested by Conway
/// You need 3 neighbour cells to make cell born and 2 or 3 cells to make cell
/// stay alive Game is over in two cases: 1) there are no alive cells 2) world
/// repeated one of previous generations
///
class ConwayRules : public GameRules {
public:
  /// @brief Get cell state in next generation
  ///
  /// @param input cell
  ///
  /// @return returns true if cell would be alive, otherwise false
  bool GetNewCellState(const Cell &cell) const override;
  /// @brief Get index of cell in the world
  ///
  /// @param current_index index which we are interested in, max_index border
  /// value(maximum of rows/columns)
  ///
  /// @return set index according to border rules
  void GetCellIndex(std::int32_t &current_index,
                    const std::uint32_t &max_index) const override;
  /// @brief Get the status of game (is it over)
  ///
  /// @param alive_cells_count count of alive cells in generation,
  /// equal_worlds_count count of generations which are equal to current,
  /// generations_count current generations count
  ///
  /// @return true if game is over, false otherwise
  bool IsGameOver(const std::uint64_t &alive_cells_count,
                  const std::uint32_t &equal_worlds_count,
                  const std::uint32_t &generations_count) const override;

private:
  /// @brief Count of neighbours for cell surviving
  const std::set<std::uint32_t> cSurvivalCount = {2, 3};
  /// @brief Count of neighbours for cell rebirth
  const std::set<std::uint32_t> cRebirthCount = {3};
  /// @brief Minimum amount of cells to not stop the game
  const std::uint64_t cMinAliveCellsCount = 1;
  /// @brief Maximum number of equal worlds to not stop the game
  const std::uint32_t cMaxEqualWorldsCount = 0;
  /// @brief Maximum number of generations, after this game stops
  const std::uint32_t cMaxGenerations = 20;
  /// @brief Borders rule
  const CellBordersRule borders_rule = CellBordersRule::RingBorders;
};

#endif // INCLUDE_CONWAY_RULES_H_
