///
/// @file
/// @copyright Copyright (C) 2020
///
#ifndef INCLUDE_WORLDHASHER_H_
#define INCLUDE_WORLDHASHER_H_
#include <cstdint>
#include <mutex>
#include <set>
#include <vector>

using WorldHash = std::vector<std::uint32_t>;

///
/// @brief The WorldHasher store hash for current generation and all previous
/// hashes The WorldHasher divides all world to segments with cCellsInOneHash
/// length
///
class WorldHasher {
public:
  /// @brief The WorldHasher get rows and columns to divide world into segments
  WorldHasher(const std::uint32_t rows, const std::uint32_t columns);
  /// @brief updates current hash according to new cell value (alive)
  void UpdateCellAlive(const std::uint32_t row, const std::uint32_t column);
  /// @brief updates current hash according to new cell value (died)
  void UpdateCellDied(const std::uint32_t row, const std::uint32_t column);
  /// @brief calling to UpdateHash indicates that world is generated, check if
  /// hash already exists
  void UpdateHash();
  /// @brief returns count of equal hashes in all generations
  std::uint32_t EqualHashCount();

private:
  /// @brief calculates index in hash vector, to which this row and column is
  /// attached In case input is incorrect, returns -1
  std::int32_t GetHasherIndex(const std::uint32_t row,
                              const std::uint32_t column);

  /// @brief hash of current generation
  WorldHash hash;
  /// @brief set of all previous hashes (for all generations)
  std::set<WorldHash> hashes;
  /// @brief count of equal hashes (in all generations)
  std::uint32_t equal_hash_count;
  /// @brief mutex to update hash from several threads
  std::mutex hash_mutex;

  /// @brief rows, columns in world are used to check to which segment is cell
  /// attached
  const std::uint32_t cRows, cColumns;
  /// @brief segment length (in cells), e.g. segment = 32 cells
  const std::uint32_t cCellsInOneHash = 32;
};

#endif // INCLUDE_WORLDHASHER_H_
