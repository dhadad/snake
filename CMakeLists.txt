cmake_minimum_required(VERSION 3.0.0)
project(chess VERSION 0.1.0)

set(MTM_FLAGS_DEBUG "-std=c++11 -Wall -Werror -g -pedantic-errors -Igfx -lX11 -lm -pthread")
set(MTM_FLAGS_RELEASE "-std=c++11 -Wall -Werror -pedantic-errors -Igfx -lX11 -lm -pthread -DNDEBUG")
include_directories(gfx)
link_directories(gfx)
set(CMAKE_CXX_FLAGS ${MTM_FLAGS_DEBUG})

add_executable(snake main.cpp vertex.cpp snake.cpp board.cpp)
target_link_libraries(snake gfx)