include_directories(../include)
add_library (game_of_life_lib cell.cpp world.cpp drawer/world_console_drawer.cpp game_of_life.cpp drawer/world_drawer_factory.cpp rules/rules_factory.cpp rules/conway_rules.cpp
        initial_figures/initial_figure.cpp world_hasher.cpp)

add_executable (game_of_life main.cpp)

find_package(Boost COMPONENTS system filesystem thread REQUIRED)

target_link_libraries (game_of_life game_of_life_lib ${Boost_SYSTEM_LIBRARY} ${Boost_FILESYSTEM_LIBRARY} Boost::thread -lpthread -lrt)
set_target_properties( game_of_life game_of_life_lib
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
