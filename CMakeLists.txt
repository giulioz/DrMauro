cmake_minimum_required(VERSION 3.0)
project(DrMauro)

add_subdirectory(deps/libsoundio)
add_subdirectory(deps/SDL-mirror)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89 -pedantic -Wall -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DrMauro_SOURCE_DIR}/cmake")
#find_package(SDL2 REQUIRED)

file(GLOB SOURCES src/*.c src/*/*.c)
file(GLOB INCLUDE_DIRS include/*)
file(GLOB INCLUDES include/*.h include/*/*.h)

include_directories(
        ${DrMauro_SOURCE_DIR}/include
        ${DrMauro_SOURCE_DIR}/include/assets
        ${DrMauro_SOURCE_DIR}/include/graphics
        ${DrMauro_SOURCE_DIR}/include/SDL
        ${DrMauro_SOURCE_DIR}/include/states
        ${DrMauro_SOURCE_DIR}/include/gamelogic
        ${DrMauro_SOURCE_DIR}/include/audio
        ${SDL2_INCLUDE_DIR}
        ${libsoundio_SOURCE_DIR}
        ${libsoundio_BINARY_DIR}
)

add_executable(DrMauro ${SOURCES} ${INCLUDES})
target_link_libraries(
    DrMauro
    m
    libsoundio_shared
    ${SDL2_LIBRARY}
)

add_subdirectory(tools)
