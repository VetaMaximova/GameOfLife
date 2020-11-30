///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "rules/conway_rules.h"
#include "world.h"

#include <gtest/gtest.h>

using cell_coordinates = std::pair<std::uint32_t, std::uint32_t>;

struct TestCase_World {
  std::string name;
  // set up inputs
  std::uint32_t rows;
  std::uint32_t columns;
  std::vector<cell_coordinates> alive_cells;
  std::vector<cell_coordinates> neighbours_for_alive_cells;
  // expected
  std::uint32_t expected_alive_cells_count;
  std::vector<std::uint32_t> expected_neighbour_counts;
};

class WorldTestFixture : public ::testing::Test,
                         public ::testing::WithParamInterface<TestCase_World> {
protected:
  ConwayRules game_rules;
};

INSTANTIATE_TEST_CASE_P(
    WorldTestFixture_CellsTest, WorldTestFixture,
    ::testing::Values(
        TestCase_World{
            "NonSquareWorld",
            6,
            10,
            {std::make_pair(1, 1), std::make_pair(2, 3), std::make_pair(4, 4)},
            {std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2),
             std::make_pair(1, 0), std::make_pair(2, 0), std::make_pair(2, 1),
             std::make_pair(2, 2), std::make_pair(1, 2), std::make_pair(3, 2),
             std::make_pair(1, 3), std::make_pair(1, 4), std::make_pair(2, 4),
             std::make_pair(3, 4), std::make_pair(3, 3), std::make_pair(4, 3),
             std::make_pair(5, 3), std::make_pair(5, 4), std::make_pair(5, 5),
             std::make_pair(4, 5), std::make_pair(3, 5)},
            3,
            {1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1}},
        TestCase_World{
            "SquareWorldWithEqualCells",
            10,
            10,
            {std::make_pair(4, 4), std::make_pair(2, 3), std::make_pair(4, 4)},
            {std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(0, 2),
             std::make_pair(1, 0), std::make_pair(2, 0), std::make_pair(2, 1),
             std::make_pair(2, 2), std::make_pair(1, 2), std::make_pair(3, 2),
             std::make_pair(1, 3), std::make_pair(1, 4), std::make_pair(2, 4),
             std::make_pair(3, 4), std::make_pair(3, 3), std::make_pair(4, 3),
             std::make_pair(5, 3), std::make_pair(5, 4), std::make_pair(5, 5),
             std::make_pair(4, 5), std::make_pair(3, 5)},
            2,
            {0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1}}));

TEST_P(WorldTestFixture, InitialStateTest) {
  // Given
  auto param{GetParam()};

  World world(param.rows, param.columns);

  // Expected
  EXPECT_EQ(world.GetRowCount(), param.rows);
  EXPECT_EQ(world.GetColumnCount(), param.columns);
  EXPECT_EQ(world.GetAliveCellsCount(), 0);
  EXPECT_EQ(world.GetEqualWorldsCount(), 0);
}

TEST_P(WorldTestFixture, MakeAliveTest) {
  // Given
  auto param{GetParam()};
  World world(param.rows, param.columns);

  for (const auto &alive_cell : param.alive_cells) {
    world.MakeCellAlive(alive_cell.first, alive_cell.second, game_rules);
  }

  // Expected
  EXPECT_EQ(world.GetAliveCellsCount(), param.expected_alive_cells_count);

  for (std::int32_t neighbour_cell_number = 0;
       neighbour_cell_number < param.neighbours_for_alive_cells.size();
       neighbour_cell_number++) {
    const auto &cell = world.GetCellAt(
        param.neighbours_for_alive_cells[neighbour_cell_number].first,
        param.neighbours_for_alive_cells[neighbour_cell_number].second);
    EXPECT_EQ(cell.GetAliveNeighboursCount(),
              param.expected_neighbour_counts[neighbour_cell_number]);
  }
}

TEST_P(WorldTestFixture, MakeDiedTest) {
  // Given
  auto param{GetParam()};
  World world(param.rows, param.columns);

  for (const auto &alive_cell : param.alive_cells) {
    world.MakeCellAlive(alive_cell.first, alive_cell.second, game_rules);
  }

  for (const auto &alive_cell : param.alive_cells) {
    world.MakeCellDied(alive_cell.first, alive_cell.second, game_rules);
  }

  // Expected
  EXPECT_EQ(world.GetAliveCellsCount(), 0);

  for (std::int32_t neighbour_cell_number = 0;
       neighbour_cell_number < param.neighbours_for_alive_cells.size();
       neighbour_cell_number++) {
    const auto &cell = world.GetCellAt(
        param.neighbours_for_alive_cells[neighbour_cell_number].first,
        param.neighbours_for_alive_cells[neighbour_cell_number].second);
    EXPECT_EQ(cell.GetAliveNeighboursCount(), 0);
  }
}

TEST_P(WorldTestFixture, WorldHashTest) {
  // Given
  auto param{GetParam()};
  World world(param.rows, param.columns);
  EXPECT_EQ(world.GetEqualWorldsCount(), 0);

  for (const auto &alive_cell : param.alive_cells) {
    world.MakeCellAlive(alive_cell.first, alive_cell.second, game_rules);
  }

  world.UpdateHash();
  EXPECT_EQ(world.GetEqualWorldsCount(), 0);

  for (const auto &alive_cell : param.alive_cells) {
    world.MakeCellDied(alive_cell.first, alive_cell.second, game_rules);
  }

  world.UpdateHash();
  EXPECT_EQ(world.GetEqualWorldsCount(), 0);

  for (const auto &alive_cell : param.alive_cells) {
    world.MakeCellAlive(alive_cell.first, alive_cell.second, game_rules);
  }

  world.UpdateHash();
  EXPECT_EQ(world.GetEqualWorldsCount(), 1);
}
