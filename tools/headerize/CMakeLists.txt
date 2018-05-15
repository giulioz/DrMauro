cmake_minimum_required(VERSION 3.0)
project(headerize)

set(CMAKE_BUILD_TYPE Debug)

add_executable(headerize headerize.cpp headerize.h)
target_link_libraries(
        headerize
        m
)
