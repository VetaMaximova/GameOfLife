///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "game_of_life.h"

#include <gtest/gtest.h>

struct TestCase_GameOfLife {
  std::string name;
  // set up inputs
  std::uint32_t rows;
  std::uint32_t columns;
  GameOfLifeInitialState initial_figure;
  // expected
  std::uint32_t generations_count;
};

class GameOfLifeTestFixture
    : public ::testing::Test,
      public ::testing::WithParamInterface<TestCase_GameOfLife> {
protected:
  std::uint32_t generations_count = 0;
};

INSTANTIATE_TEST_CASE_P(
    GameOfLifeTest, GameOfLifeTestFixture,
    ::testing::Values(
        TestCase_GameOfLife{"LineBigSceneTest", 100, 100,
                            GameOfLifeInitialState::CenterLine, 2},
        TestCase_GameOfLife{"LineLittleSceneTest", 5, 5,
                            GameOfLifeInitialState::CenterLine, 2},
        TestCase_GameOfLife{"PointBigSceneTest", 100, 100,
                            GameOfLifeInitialState::CenterPoint, 1},
        TestCase_GameOfLife{"PointLittleSceneTest", 5, 5,
                            GameOfLifeInitialState::CenterPoint, 1}));

TEST_P(GameOfLifeTestFixture, GameOfLifeTest) {
  // Given
  auto param{GetParam()};

  GameOfLife game(param.rows, param.columns);
  game.FillInitialPicture(param.initial_figure);

  while (!game.IsGameOver()) {
    game.ExecuteNextGeneration();
    generations_count++;
  }

  EXPECT_EQ(generations_count, param.generations_count);
}
