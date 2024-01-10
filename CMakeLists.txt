cmake_minimum_required(VERSION 3.25)

project(ttt)

set(CMAKE_C_STANDARD 99)

include(FindCurses)

add_executable(${PROJECT_NAME}
    main.c
    game.c
    ai.c
    tgui.c
    tictactoe.c
    utils.c
)

target_link_libraries(${PROJECT_NAME} ${CURSES_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${CURSES_INCLUDE_DIRS}/ncurses)
target_link_options(${PROJECT_NAME} PRIVATE -static)

add_definitions(-DNCURSES_STATIC)
