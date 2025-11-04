cmake_minimum_required(VERSION 3.28)
project(SnakeGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(snake_game
    src/main.cpp
    src/core.cpp
)

target_include_directories(snake_game PRIVATE include)

# Link ncurses library
target_link_libraries(snake_game ncurses)

# Enable warnings
target_compile_options(snake_game PRIVATE
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -pedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)