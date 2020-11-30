cmake_minimum_required(VERSION 3.10)
set (CMAKE_CXX_STANDARD 11)
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

project(game_of_life)

add_subdirectory (src)
add_subdirectory (test)

enable_testing ()
add_test (NAME game_of_life_test COMMAND Test)
