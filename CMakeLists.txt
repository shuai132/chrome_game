cmake_minimum_required(VERSION 3.2)
project(chrome_game_demo)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(engine)
include_directories(.)
add_executable(${PROJECT_NAME}_curses main.cpp game/chrome_game.cpp impl/game_engine_port_curses.cpp)
target_link_libraries(${PROJECT_NAME}_curses game_engine ncurses)

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
add_executable(${PROJECT_NAME}_opengl main.cpp game/chrome_game.cpp impl/game_engine_port_opengl.cpp impl/Adafruit_GFX.cpp impl/glcdfont.c)

target_include_directories(${PROJECT_NAME}_opengl PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME}_opengl game_engine glfw ${OPENGL_LIBRARIES})
target_compile_definitions(${PROJECT_NAME}_opengl PRIVATE -DGL_SILENCE_DEPRECATION)
