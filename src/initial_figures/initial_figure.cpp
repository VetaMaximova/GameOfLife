///
/// @file
/// @copyright Copyright (C) 2020
///
#include "initial_figures/initial_figure.h"

#include <iostream>

InitialFigure::InitialFigure(const std::uint32_t bounding_box_x,
                             const std::uint32_t bounding_box_y)
    : max_x(bounding_box_x), max_y(bounding_box_y), mte(rd()) {
  figure_points.clear();
}

void InitialFigure::BuildRandomPoint() {
  std::uniform_int_distribution<std::uint32_t> dist_x(0, max_x - 1),
      dist_y(0, max_y - 1);
  BuildPoint(dist_x(mte), dist_y(mte));
}

void InitialFigure::BuildRandomLine() {
  std::uniform_int_distribution<std::uint32_t> dist_x(0, max_x - 1),
      dist_y(0, max_y - 1);
  BuildLine(dist_x(mte), dist_y(mte));
}

void InitialFigure::BuildRandomGlider() {
  std::uniform_int_distribution<std::uint32_t> dist_x(0, max_x - 1),
      dist_y(0, max_y - 1);
  std::uint32_t x = dist_x(mte);
  std::uint32_t y = dist_y(mte);
  figure_points = {{(x + 2) % max_x, y},
                   {(x + 2) % max_x, (y + 1) % max_y},
                   {(x + 2) % max_x, (y + 2) % max_y},
                   {(x + 1) % max_x, (y + 2) % max_y},
                   {x, (y + 1) % max_y}};
}

void InitialFigure::BuildRandom() {
  std::uniform_int_distribution<std::uint32_t> dist_x(0, max_x - 1),
      dist_y(0, max_y - 1), dist_points_count(0, max_x * max_y - 1);
  std::uint32_t points_count = dist_points_count(mte);

  for (std::uint32_t point = 0; point < points_count; point++) {
    std::uint32_t x = dist_x(mte);
    std::uint32_t y = dist_y(mte);
    figure_points.push_back({x, y});
  }
}

void InitialFigure::BuildPoint(const std::uint32_t x, const std::uint32_t y) {
  figure_points.push_back({x, y});
}

void InitialFigure::BuildLine(const std::uint32_t initial_x,
                              const std::uint32_t initial_y) {
  figure_points = {{initial_x, initial_y},
                   {initial_x, (initial_y + 1) % max_y},
                   {initial_x, (initial_y + 2) % max_y}};
}

const std::vector<Point> &InitialFigure::GetPoints() const {
  return figure_points;
}
