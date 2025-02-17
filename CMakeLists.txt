cmake_minimum_required(VERSION 3.30)
project(console_25d C)

set(CMAKE_C_STANDARD 11)

add_executable(console_25d src/main.c
        src/virtual_screen.c
        src/virtual_screen.h
        src/math.c
        src/math.h
        src/debug.c
        src/debug.h
)

target_link_libraries(console_25d m)
target_link_libraries(console_25d curses)
