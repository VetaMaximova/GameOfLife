///
/// @file
/// @copyright Copyright (C) 2020
///
#include "drawer/world_drawer_factory.h"
#include "drawer/world_console_drawer.h"

std::unique_ptr<WorldDrawer> WorldDrawerFactory::MakeWorldDrawer() {
  return std::unique_ptr<WorldDrawer>(new WorldConsoleDrawer());
}
