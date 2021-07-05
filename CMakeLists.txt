cmake_minimum_required(VERSION 3.2)
project(chrome_game)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(game)
include_directories(.)
add_executable(chrome_game main.cpp game/chrome_game.cpp impl/game_engine_port_desktop.cpp)
target_link_libraries(chrome_game game_engine ncurses)
