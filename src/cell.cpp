///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "cell.h"

namespace detail {
CellData::CellData() {
  is_alive = 0;
  neighbours_count = 0;
}
} // namespace detail

bool Cell::IsAlive() const {
  std::lock_guard<std::mutex> lock(cell_mutex);
  return data.is_alive;
}

std::uint8_t Cell::GetAliveNeighboursCount() const {
  std::lock_guard<std::mutex> lock(cell_mutex);
  return data.neighbours_count;
}

void Cell::MakeAlive() {
  std::lock_guard<std::mutex> lock(cell_mutex);
  data.is_alive = 1;
}

void Cell::MakeDied() {
  std::lock_guard<std::mutex> lock(cell_mutex);
  data.is_alive = 0;
}

void Cell::AddNeighbour() {
  std::lock_guard<std::mutex> lock(cell_mutex);
  if (data.neighbours_count < cMaxNeighboursCount) {
    data.neighbours_count++;
  }
}

void Cell::RemoveNeighbour() {
  std::lock_guard<std::mutex> lock(cell_mutex);
  if (data.neighbours_count > 0) {
    data.neighbours_count--;
  }
}
