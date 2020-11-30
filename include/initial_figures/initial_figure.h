///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_INITIAL_FIGURES_INITIAL_FIGURE_H_
#define INCLUDE_INITIAL_FIGURES_INITIAL_FIGURE_H_
#include <cstdint>
#include <random>
#include <vector>

///
/// @brief The Point describes point at 2d plot
///
struct Point {
  std::uint32_t x;
  std::uint32_t y;
};

///
/// @brief The InitialFigure class creates figures of selected types
///
class InitialFigure {
public:
  InitialFigure(const std::uint32_t bounding_box_x,
                const std::uint32_t bounding_box_y);
  /// @brief Build single random point
  void BuildRandomPoint();
  /// @brief Build single random line
  void BuildRandomLine();
  /// @brief Build point at specific position
  void BuildPoint(const std::uint32_t x, const std::uint32_t y);
  /// @brief Build line starting at specific point
  void BuildLine(const std::uint32_t initial_x, const std::uint32_t initial_y);
  /// @brief Build random glider
  void BuildRandomGlider();
  /// @brief Build random set of points
  void BuildRandom();
  /// @brief Returns points of created figure
  const std::vector<Point> &GetPoints() const;

private:
  /// @brief Points of the figure
  std::vector<Point> figure_points;
  std::random_device rd;
  std::mt19937 mte;
  /// @brief Border x and y values
  std::uint32_t max_x, max_y;
};

#endif // INCLUDE_INITIAL_FIGURES_INITIAL_FIGURE_H_
