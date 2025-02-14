cmake_minimum_required(VERSION 3.30)
project(console_25d C)

set(CMAKE_C_STANDARD 11)

add_executable(console_25d main.c
        virtual_screen.c
        virtual_screen.h
        math.c
        math.h
)

target_link_libraries(console_25d m)
