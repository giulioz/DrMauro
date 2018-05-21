cmake_minimum_required(VERSION 3.0)
project(charmapgen)

set(CMAKE_BUILD_TYPE Debug)

add_executable(charmapgen charmapgen.c)
target_link_libraries(
        charmapgen
)