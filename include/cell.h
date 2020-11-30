///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_CELL_H_
#define INCLUDE_CELL_H_
#include <cstdint>
#include <mutex>

namespace detail {
///
/// @brief The CellData store all information to process cell in new generation
/// is_alive if cell is alive or not, neighbours count
///
struct CellData {
  CellData();
  std::uint8_t is_alive : 1;
  std::uint8_t neighbours_count : 4;
};
} // namespace detail

///
/// @brief The Cell store and manipulate cell information
///
class Cell {
public:
  /// @brief True if cell is alive
  bool IsAlive() const;
  /// @brief Return count of cell's neighbours
  std::uint8_t GetAliveNeighboursCount() const;
  /// @brief make cell alive
  void MakeAlive();
  /// @brief make cell died
  void MakeDied();
  /// @brief add neighbour to cell
  void AddNeighbour();
  /// @brief remove neighbour from cell
  void RemoveNeighbour();

private:
  /// @brief cell data
  detail::CellData data;
  /// @brief mutex to make all operations with a cell thread-safe
  mutable std::mutex cell_mutex;

  /// @brief the maximum number of neighbour cells
  const std::uint8_t cMaxNeighboursCount = 8;
};

#endif // INCLUDE_CELL_H_
