///
/// @file
/// @copyright Copyright (C) 2020
///
#include "world_hasher.h"

#include <iostream>
#include <math.h>

WorldHasher::WorldHasher(const std::uint32_t rows, const std::uint32_t columns)
    : cRows(rows), cColumns(columns) {
  std::uint64_t cells_count = cRows * cColumns;
  hash.resize(round(cells_count / (float)cCellsInOneHash));
  equal_hash_count = 0;
}

std::int32_t WorldHasher::GetHasherIndex(const std::uint32_t row,
                                         const std::uint32_t column) {
  std::uint32_t hasher_index = (row * cColumns + column) / cCellsInOneHash;
  if (hasher_index > hash.size()) {
    std::cerr << "Can't calculate correct index for row " << row << ", column "
              << column << std::endl;
    return -1;
  }

  return hasher_index;
}

void WorldHasher::UpdateCellAlive(const std::uint32_t row,
                                  const std::uint32_t column) {
  std::int32_t hasher_index = GetHasherIndex(row, column);
  if (hasher_index < 0) {
    return;
  }
  std::lock_guard<std::mutex> lock(hash_mutex);
  hash[hasher_index] |= 1 << ((row * cColumns + column) % cCellsInOneHash);
}

void WorldHasher::UpdateCellDied(const std::uint32_t row,
                                 const std::uint32_t column) {
  std::int32_t hasher_index = GetHasherIndex(row, column);
  if (hasher_index < 0) {
    return;
  }
  std::lock_guard<std::mutex> lock(hash_mutex);
  hash[hasher_index] &=
      (0xFFFFFFFF ^ (1 << ((row * cColumns + column) % cCellsInOneHash)));
}

void WorldHasher::UpdateHash() {
  if (hashes.find(hash) != hashes.end()) {
    equal_hash_count++;
  } else {
    hashes.insert(hash);
  }
}

std::uint32_t WorldHasher::EqualHashCount() { return equal_hash_count; }
