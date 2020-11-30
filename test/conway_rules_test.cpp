///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "rules/conway_rules.h"

#include <gtest/gtest.h>

struct TestCase_Conway_Alive {
  std::string name;
  // set up inputs
  std::uint32_t neighbours_count;
  bool is_alive;
  // expected
  bool stay_alive;
};

class ConwayTestAliveFixture
    : public ::testing::Test,
      public ::testing::WithParamInterface<TestCase_Conway_Alive> {
protected:
  ConwayRules game_rules;
};

INSTANTIATE_TEST_CASE_P(
    ConwayTestFixture_CellsTest, ConwayTestAliveFixture,
    ::testing::Values(
        TestCase_Conway_Alive{"CellShouldBorn", 3, false, true},
        TestCase_Conway_Alive{"CellShouldNotBornLess3", 2, false, false},
        TestCase_Conway_Alive{"CellShouldNotBornMore3", 4, false, false},
        TestCase_Conway_Alive{"CellShouldDieMore3", 4, true, false},
        TestCase_Conway_Alive{"CellShouldDieLess2", 1, true, false},
        TestCase_Conway_Alive{"CellShouldStayAlive3", 3, true, true},
        TestCase_Conway_Alive{"CellShouldStayAlive2", 2, true, true}));

TEST_P(ConwayTestAliveFixture, CellStateTest) {
  // Given
  auto param{GetParam()};

  Cell cell;
  if (param.is_alive) {
    cell.MakeAlive();
  }

  for (std::uint8_t neighbour = 0; neighbour < param.neighbours_count;
       neighbour++) {
    cell.AddNeighbour();
  }

  // Expected
  EXPECT_EQ(game_rules.GetNewCellState(cell), param.stay_alive);
}

struct TestCase_Conway_GameOver {
  std::string name;
  // set up inputs
  std::uint32_t alive_cells_count;
  std::uint32_t equal_worlds_count;
  std::uint32_t generations_count;
  // expected
  bool is_game_over;
};

class ConwayGameOverTestFixture
    : public ::testing::Test,
      public ::testing::WithParamInterface<TestCase_Conway_GameOver> {
protected:
  ConwayRules game_rules;
};

INSTANTIATE_TEST_CASE_P(
    ConwayTestFixture_GameOverTest, ConwayGameOverTestFixture,
    ::testing::Values(
        TestCase_Conway_GameOver{"GameOverIfCellsDead", 0, 0, 10, true},
        TestCase_Conway_GameOver{"GameNotOverIfCellsAlive", 10, 0, 5, false},
        TestCase_Conway_GameOver{"GameOverWorldIsRepeated", 100, 1, 20, true}));

TEST_P(ConwayGameOverTestFixture, GameOverTest) {
  // Given
  auto param{GetParam()};

  bool is_game_over =
      game_rules.IsGameOver(param.alive_cells_count, param.equal_worlds_count,
                            param.generations_count);

  // Expected
  EXPECT_EQ(is_game_over, param.is_game_over);
}
