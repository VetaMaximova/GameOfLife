///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#ifndef INCLUDE_GAME_OF_LIFE_H_
#define INCLUDE_GAME_OF_LIFE_H_
#include "drawer/world_drawer.h"
#include "initial_figures/initial_figure.h"
#include "rules/rules_factory.h"
#include "world.h"

#include <boost/thread.hpp>
#include <condition_variable>
#include <mutex>
#include <semaphore.h>

///
/// @brief The GameOfLifeInitialState enumerates initial states which we can
/// use to initialize world
///
enum class GameOfLifeInitialState {
  Random,
  RandomLine,
  RandomPoint,
  CenterPoint,
  CenterLine,
  Glider
};

/// @brief row, column and is_alive for cell
using CellData = std::tuple<std::uint32_t, std::uint32_t, bool>;

///
/// @brief The GameOfLife store world, rules, drawer
/// and is used to control game process
///
class GameOfLife {
public:
  /// @brief The GameOfLife is initialized with rows and columns count. Infinity
  /// of boards could be achieved by border rules
  GameOfLife(const std::uint32_t rows, const std::uint32_t columns);
  /// @brief when game is finished, all threads are stopped
  ~GameOfLife();
  /// @brief Draw with default drawer
  void Draw();
  /// @brief Calculate next generation
  void ExecuteNextGeneration();
  /// @brief Set initial state to world
  void FillInitialPicture(const GameOfLifeInitialState &state);
  /// @brief Check if game is over
  bool IsGameOver();

private:
  /// @brief In case of multithread run, update cell state in one of (several)
  /// threads
  void ProcessCellsThread(std::uint32_t thread_num);
  /// @brief Run the generation multithreaded
  void ExecuteNextGenerationMultithreaded();
  /// @brief Run the generation in single thread
  void ExecuteNextGenerationSinglehread();
  /// @brief Call updates of the world with new cell states (add alive, delete
  /// alive)
  void
  UpdateWorldWithNewCellStates(const std::vector<CellData> &new_cell_states);

  /// @brief world for the game
  World world;
  /// @brief helper to create initial world
  InitialFigure initial_figure;
  /// @brief default drawer
  std::unique_ptr<WorldDrawer> drawer;
  /// @brief default rules
  std::unique_ptr<GameRules> rules;
  /// @brief current count of generations
  std::uint32_t generations_count;
  /// @brief If true run generation of new world in several threads
  bool multithread;
  /// @brief thread group
  boost::thread_group thread_group;
  /// @brief semaphores which are sent to each thread to srart processing
  std::vector<sem_t> start_cell_process_semaphores;
  /// @brief if true, game is over and threads should be stopped
  std::atomic<bool> stop_threads;
  /// @brief count of threads which have already calculated result
  std::uint32_t thread_finished_count;
  /// @brief count of threads which have already prepared data for calculations
  std::uint32_t threads_preparation_finished_count;
  /// @brief count of threads which we execute
  std::uint32_t threads_count;
  /// @brief conditional wait while cell states are processed
  std::condition_variable conditional_wait_cell_states;
  /// @brief mutex for conditional wait while cell states are processed
  std::mutex conditional_wait_cell_states_mutex;
  /// @brief conditional wait for finish computations
  std::condition_variable conditional_wait_cell_processed;
  /// @brief mutex for conditional wait of finish computations
  std::mutex conditional_wait_cell_processed_mutex;

  /// @brief maximum amount of threads in game
  const std::uint32_t cMaxThreadCount = 100;
  /// @brief minimum points in game, when we start multithreading
  const std::uint32_t cMinPointsForMultithreading = 40;
};

#endif // INCLUDE_GAME_OF_LIFE_H_
