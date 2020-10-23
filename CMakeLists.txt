cmake_minimum_required(VERSION 3.17)
project(FlappyBird C)

find_package(raylib 2.5.0 REQUIRED)

set(CMAKE_C_STANDARD 99)

add_executable(FlappyBird main.c)

target_link_libraries(FlappyBird raylib)
