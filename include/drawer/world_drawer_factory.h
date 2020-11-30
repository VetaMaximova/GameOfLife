///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_WORLD_DRAWER_FACTORY_H_
#define INCLUDE_WORLD_DRAWER_FACTORY_H_
#include "world_drawer.h"

///
/// @brief The WorldDrawerFactory creates default drawer
///
class WorldDrawerFactory {
public:
  /// @brief Draws world cells
  ///
  /// @param cells are array of world cells
  ///
  /// @return returns unique pointer to created drawer
  static std::unique_ptr<WorldDrawer> MakeWorldDrawer();
};

#endif // INCLUDE_WORLD_DRAWER_FACTORY_H_
