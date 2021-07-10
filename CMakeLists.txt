cmake_minimum_required(VERSION 3.2)
project(chrome_game_demo)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(engine)
include_directories(.)
add_executable(${PROJECT_NAME} main.cpp game/chrome_game.cpp impl/game_engine_port_desktop.cpp)
target_link_libraries(${PROJECT_NAME} game_engine ncurses)

