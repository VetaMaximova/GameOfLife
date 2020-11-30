find_package(GTest REQUIRED)
include_directories(../include)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

add_executable (game_of_life_test world_test.cpp cell_test.cpp conway_rules_test.cpp game_of_life_test.cpp)
target_link_libraries(game_of_life_test GTest::GTest GTest::Main game_of_life_lib ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} Boost::thread -lpthread -lrt)
