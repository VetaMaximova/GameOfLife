///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_CONSOLE_DRAWER_H_
#define INCLUDE_CONSOLE_DRAWER_H_
#include "world_drawer.h"

///
/// @brief The WorldConsoleDrawer draws cells of the world generation in
/// standard output
///
class WorldConsoleDrawer : public WorldDrawer {
public:
  void DrawCells(const WorldCells &cells) override;

private:
  /// @brief Draws a heading line
  ///
  /// @param heading_size is a size if heading line
  void DrawHeadingLine(const std::uint32_t heading_size);
  /// @brief Colors to draw alive and dead cells
  const std::string cRedColor = "\033[1;31m", cGreenColor = "\033[1;32m",
                    cNoColor = "\033[0m";
};

#endif // INCLUDE_CONSOLE_DRAWER_H_
