cmake_minimum_required(VERSION 3.30)
project(console_25d C)

set(CMAKE_C_STANDARD 11)

add_executable(console_25d src/main.c
        src/screen/virtual_screen.c
        src/screen/virtual_screen.h
        src/screen/debug.c
        src/screen/debug.h
        src/math/vector.h
        src/math/matrix.h
        src/math/shape.c
        src/math/shape.h
        src/screen/virtual_screen_data.h
        src/math/matrix_op.c
        src/math/matrix_op.h
        src/math/vector_op.c
        src/math/vector_op.h
)

target_link_libraries(console_25d m)
target_link_libraries(console_25d curses)
