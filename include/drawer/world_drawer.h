///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_DRAWER_H_
#define INCLUDE_DRAWER_H_
#include "../world.h"

///
/// @brief The WorldDrawer draws cells of the world generation
///
class WorldDrawer {
public:
  /// @brief Draws world cells
  ///
  /// @param cells are array of world cells
  virtual void DrawCells(const WorldCells &cells) = 0;
};

#endif // INCLUDE_DRAWER_H_
