# Was in use only for building project on Windows

cmake_minimum_required(VERSION 3.2)

project(ttt)

set(CMAKE_C_STANDARD 99)

set(CURSES_NEED_NCURSES TRUE)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
include_directories(${CURSES_INCLUDE_DIR}/ncurses)

add_definitions(-DNCURSES_STATIC)

add_executable(${PROJECT_NAME}
    src/main.c
    src/game.c
    src/ai.c
    src/tgui.c
    src/tictactoe.c
    src/utils.c
)

target_link_libraries(${PROJECT_NAME} ncurses)

