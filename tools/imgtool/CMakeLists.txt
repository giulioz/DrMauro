cmake_minimum_required(VERSION 3.0)
project(imgtool)

set(CMAKE_BUILD_TYPE Debug)

add_executable(imgtool imgtool.cpp imgtool.h)
target_link_libraries(
        imgtool
        m
)
