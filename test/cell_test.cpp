///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "cell.h"

#include <gtest/gtest.h>

TEST(CellTest, InitialStateTest) {
  Cell test_cell;
  EXPECT_FALSE(test_cell.IsAlive());
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 0);
}

TEST(CellTest, RemoveZeroNeighboursTest) {
  Cell test_cell;
  test_cell.RemoveNeighbour();
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 0);
}

TEST(CellTest, MakeAliveTest) {
  Cell test_cell;
  EXPECT_FALSE(test_cell.IsAlive());
  test_cell.MakeAlive();
  EXPECT_TRUE(test_cell.IsAlive());
}

TEST(CellTest, Add8NighboursTest) {
  Cell test_cell;
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 0);
  for (std::uint8_t neighbour = 0; neighbour < 8; neighbour++) {
    test_cell.AddNeighbour();
    EXPECT_EQ(test_cell.GetAliveNeighboursCount(), neighbour + 1);
  }
}

TEST(CellTest, AddMoreThan8NighboursTest) {
  Cell test_cell;
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 0);
  for (std::uint8_t neighbour = 0; neighbour < 8; neighbour++) {
    test_cell.AddNeighbour();
    EXPECT_EQ(test_cell.GetAliveNeighboursCount(), neighbour + 1);
  }
  test_cell.AddNeighbour();
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 8);
}

TEST(CellTest, MakeDiedTest) {
  Cell test_cell;
  EXPECT_FALSE(test_cell.IsAlive());
  test_cell.MakeAlive();
  EXPECT_TRUE(test_cell.IsAlive());
  test_cell.MakeDied();
  EXPECT_FALSE(test_cell.IsAlive());
}

TEST(CellTest, Remove8NeighboursTest) {
  Cell test_cell;
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 0);
  for (std::uint8_t neighbour = 0; neighbour < 8; neighbour++) {
    test_cell.AddNeighbour();
  }

  for (std::uint8_t neighbour = 8; neighbour > 0; neighbour--) {
    test_cell.RemoveNeighbour();
    EXPECT_EQ(test_cell.GetAliveNeighboursCount(), neighbour - 1);
  }

  test_cell.RemoveNeighbour();
  EXPECT_EQ(test_cell.GetAliveNeighboursCount(), 0);
}
