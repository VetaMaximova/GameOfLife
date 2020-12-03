///
/// @file
/// @copyright Copyright (C) 2020, Bayerische Motoren Werke Aktiengesellschaft
/// (BMW AG)
///
#include "game_of_life.h"
#include "drawer/world_drawer_factory.h"

#include <iostream>
#include <map>
#include <math.h>
#include <thread>

GameOfLife::GameOfLife(const std::uint32_t rows, const std::uint32_t columns)
    : world(rows, columns), initial_figure(rows, columns),
      generations_count(0) {
  drawer = WorldDrawerFactory::MakeWorldDrawer();
  rules = GameRulesFactory::MakeGameRules();

  if (rows * columns > cMinPointsForMultithreading) {
    multithread = true;
    stop_threads = false;
    threads_count =
        std::min(cMaxThreadCount, std::thread::hardware_concurrency());

    start_cell_process_semaphores.resize(threads_count);
    for (std::uint32_t thread_num = 0; thread_num < threads_count;
         thread_num++) {
      int sem_init_result =
          sem_init(&start_cell_process_semaphores[thread_num], 0, 0);
      if (sem_init_result) {
        std::cerr << "Can't create a semaphore, move to single thread"
                  << std::endl;
        multithread = false;
        threads_count = 0;
        break;
      }
      thread_group.create_thread(
          boost::bind(&GameOfLife::ProcessCellsThread, this, thread_num));
    }
  } else {
    multithread = false;
    threads_count = 0;
  }
}

GameOfLife::~GameOfLife() {
  if (multithread) {
    stop_threads.store(true);
    thread_group.join_all();
  }
}

void GameOfLife::Draw() { drawer->DrawCells(world.GetCells()); }

void GameOfLife::FillInitialPicture(const GameOfLifeInitialState &state) {
  switch (state) {
  case GameOfLifeInitialState::RandomPoint: {
    initial_figure.BuildRandomPoint();
  } break;
  case GameOfLifeInitialState::CenterPoint: {
    initial_figure.BuildPoint(world.GetRowCount() / 2,
                              world.GetColumnCount() / 2);
  } break;
  case GameOfLifeInitialState::RandomLine: {
    initial_figure.BuildRandomLine();
  } break;
  case GameOfLifeInitialState::CenterLine: {
    initial_figure.BuildLine(world.GetRowCount() / 2,
                             world.GetColumnCount() / 2);
  } break;
  case GameOfLifeInitialState::Glider: {
    initial_figure.BuildRandomGlider();
  } break;
  case GameOfLifeInitialState::Random:
  default: { initial_figure.BuildRandom(); }
  }

  world.SetInitialCells(initial_figure.GetPoints(), *rules.get());
  world.UpdateHash();
}

void GameOfLife::UpdateWorldWithNewCellStates(
    const std::vector<CellData> &new_cell_states) {
  for (const auto &new_cell_state : new_cell_states) {
    if (std::get<2>(new_cell_state)) {
      world.MakeCellAlive(std::get<0>(new_cell_state),
                          std::get<1>(new_cell_state), *rules.get());
    } else {
      world.MakeCellDied(std::get<0>(new_cell_state),
                         std::get<1>(new_cell_state), *rules.get());
    }
  }
}

void GameOfLife::ProcessCellsThread(std::uint32_t thread_num) {
  int sem_wait_result;
  struct timespec ts;
  constexpr std::uint64_t sem_wait_nanos = 1000000ULL;
  ts.tv_nsec += sem_wait_nanos;

  std::uint32_t cell_count = std::ceil(
      world.GetRowCount() * world.GetColumnCount() / (float)threads_count);
  std::uint32_t start_column = 0;
  std::uint32_t start_row = 0;

  if (world.GetColumnCount() <= cell_count) {
    start_row = thread_num * cell_count / world.GetColumnCount();
    start_column = (thread_num * cell_count) % world.GetColumnCount();
  } else {
    start_row = (thread_num * cell_count) / world.GetColumnCount();
    start_column = (thread_num * cell_count) % world.GetColumnCount();
  }

  while (!stop_threads.load()) {
    while ((sem_wait_result = sem_timedwait(
                &start_cell_process_semaphores[thread_num], &ts)) == -1 &&
           errno == EINTR)
      continue;

    if (sem_wait_result == -1) {
      continue;
    }

    std::vector<CellData> new_cell_states;

    std::uint32_t points_count = 0;
    for (std::uint32_t row = start_row; row < world.GetRowCount(); row++) {
      if (points_count > cell_count) {
        break;
      }
      for (std::uint32_t column = (row == start_row ? start_column : 0);
           column < world.GetColumnCount(); column++) {
        points_count++;
        if (points_count > cell_count) {
          break;
        }
        const auto &cell = world.GetCellAt(row, column);
        bool is_cell_alive = rules->GetNewCellState(cell);
        if (is_cell_alive == cell.IsAlive())
          continue;

        new_cell_states.push_back(std::make_tuple(row, column, is_cell_alive));
      }
    }

    {
      std::lock_guard<std::mutex> lk(conditional_wait_cell_states_mutex);
      threads_preparation_finished_count++;
    }
    conditional_wait_cell_states.notify_all();

    {
      std::unique_lock<std::mutex> lk(conditional_wait_cell_states_mutex);
      conditional_wait_cell_states.wait(lk, [this] {
        return threads_preparation_finished_count >= threads_count;
      });
    }

    UpdateWorldWithNewCellStates(new_cell_states);

    {
      std::lock_guard<std::mutex> lk(conditional_wait_cell_processed_mutex);
      thread_finished_count++;
    }
    conditional_wait_cell_processed.notify_all();
  }
}

void GameOfLife::ExecuteNextGenerationMultithreaded() {
  thread_finished_count = 0;
  threads_preparation_finished_count = 0;
  for (std::uint32_t thread_num = 0; thread_num < threads_count; thread_num++) {
    int sem_post_result = sem_post(&start_cell_process_semaphores[thread_num]);
    if (sem_post_result) {
      std::cerr << "Problems with creating a semaphore" << std::endl;
      multithread = false;
      threads_count = 0;
      ExecuteNextGenerationSinglehread();
      return;
    }
  }

  {
    std::unique_lock<std::mutex> lk(conditional_wait_cell_processed_mutex);
    conditional_wait_cell_processed.wait(
        lk, [this] { return thread_finished_count >= threads_count; });
  }
}

void GameOfLife::ExecuteNextGenerationSinglehread() {
  std::vector<CellData> new_cell_states;

  for (std::uint32_t row = 0; row < world.GetRowCount(); row++) {
    for (std::uint32_t column = 0; column < world.GetColumnCount(); column++) {
      const auto &cell = world.GetCellAt(row, column);
      bool is_cell_alive = rules->GetNewCellState(cell);
      if (is_cell_alive == cell.IsAlive())
        continue;

      new_cell_states.push_back(std::make_tuple(row, column, is_cell_alive));
    }
  }

  UpdateWorldWithNewCellStates(new_cell_states);
}

void GameOfLife::ExecuteNextGeneration() {
  if (multithread) {
    ExecuteNextGenerationMultithreaded();
  } else {
    ExecuteNextGenerationSinglehread();
  }

  world.UpdateHash();
  generations_count++;
}

bool GameOfLife::IsGameOver() {
  return rules->IsGameOver(world.GetAliveCellsCount(),
                           world.GetEqualWorldsCount(), generations_count);
}
