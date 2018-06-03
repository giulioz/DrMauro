cmake_minimum_required(VERSION 3.0)
project(DrMauro)

if (MSVC)
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
add_subdirectory(deps/libsoundio)
add_subdirectory(deps/SDL-mirror)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89 -pedantic -Wall -g")
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${DrMauro_SOURCE_DIR}/cmake")

file(GLOB SOURCES src/*.c src/*/*.c)
file(GLOB INCLUDE_DIRS include/*)
file(GLOB INCLUDES include/*.h include/*/*.h)

include_directories(
		${DrMauro_SOURCE_DIR}/deps/debugbreak
        ${DrMauro_SOURCE_DIR}/include
        ${DrMauro_SOURCE_DIR}/include/assets
        ${DrMauro_SOURCE_DIR}/include/graphics
        ${DrMauro_SOURCE_DIR}/include/SDL
        ${DrMauro_SOURCE_DIR}/include/states
        ${DrMauro_SOURCE_DIR}/include/gamelogic
        ${DrMauro_SOURCE_DIR}/include/audio
        ${DrMauro_SOURCE_DIR}/deps/SDL-mirror/include
        ${libsoundio_SOURCE_DIR}
        ${libsoundio_BINARY_DIR}
)

if (MSVC)
	add_executable(DrMauro WIN32 ${SOURCES} ${INCLUDES})
	set_target_properties(DrMauro PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:WINDOWS")
	set_target_properties(DrMauro PROPERTIES COMPILE_DEFINITIONS_DEBUG "_CONSOLE")
	set_target_properties(DrMauro PROPERTIES LINK_FLAGS_RELWITHDEBINFO "/SUBSYSTEM:WINDOWS")
	set_target_properties(DrMauro PROPERTIES COMPILE_DEFINITIONS_RELWITHDEBINFO "_CONSOLE")
	set_target_properties(DrMauro PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
	set_target_properties(DrMauro PROPERTIES LINK_FLAGS_MINSIZEREL "/SUBSYSTEM:WINDOWS")
	target_link_libraries(
		DrMauro
		libsoundio_shared
		SDL2
	)
else()
	add_executable(DrMauro ${SOURCES} ${INCLUDES})
	target_link_libraries(
		DrMauro
		m
		libsoundio_shared
		SDL2
	)
endif()

add_subdirectory(tools)

MESSAGE(${SDL2_INCLUDE_DIR})
