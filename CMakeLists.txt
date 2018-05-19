cmake_minimum_required(VERSION 3.0)
project(DrMauro)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89 -pedantic -Wall -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DrMauro_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)

include_directories(
    ${DrMauro_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIR}
)

file(GLOB SOURCES src/*.c src/*/*.c)
file(GLOB INCLUDES include/*.h include/*/*.h)

add_executable(DrMauro ${SOURCES} ${INCLUDES})
target_link_libraries(
    DrMauro
    m
    ${SDL2_LIBRARY}
)

add_subdirectory(tools)
