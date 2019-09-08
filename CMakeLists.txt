cmake_minimum_required(VERSION 3.0)

project(CarAI)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
message("Set variables")

add_executable(CarAI
	src/main.cpp
	src/math/spline.h
	src/car.h
	src/car.cpp
	src/math/vector2d.h
	src/math/vector2d.cpp

	src/NEAT/neat.h
	src/NEAT/neat.cpp
	src/NEAT/network/network.h
	src/NEAT/network/network.cpp
	src/NEAT/network/genome.h
	src/NEAT/network/genome.cpp
	src/NEAT/network/component/node.h
	src/NEAT/network/component/node.cpp
	src/NEAT/network/component/connection.h
	src/NEAT/network/component/connection.cpp
	src/NEAT/utilities/mutator.h
	src/NEAT/utilities/mutator.cpp
	src/NEAT/utilities/speciator.h
	src/NEAT/utilities/speciator.cpp
	src/NEAT/utilities/crossover.h
	src/NEAT/utilities/crossover.cpp
	src/NEAT/utilities/evolver.h
	src/NEAT/utilities/evolver.cpp
	src/NEAT/utilities/serializer.h
	src/NEAT/utilities/serializer.cpp
)

message("Added sources")

add_compile_definitions(USE_SDL2_GFX)

if (UNIX)
find_package(SDL2 REQUIRED)
message("SDL2 found")

find_library(SDL2_GFX_LIBRARY SDL2_gfx)
find_path(SDL2_GFX_INCLUDE SDL2_gfxPrimitives.h PATHS /usr/local/include/SDL2)
endif (UNIX)

if (WIN32)
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
	set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/libs/SDL2/x64/SDL2.lib;${CMAKE_SOURCE_DIR}/libs/SDL2/x64/SDL2main.lib")
	set(SDL2_GFX_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SDL2_gfx/x64/SDL2_gfx.lib")

	file(COPY ${CMAKE_SOURCE_DIR}/libs/SDL2/x64/SDL2.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin/Debug/)
	file(COPY ${CMAKE_SOURCE_DIR}/libs/SDL2_gfx/x64/SDL2_gfx.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin/Debug/)
else ()
	set(SDL2_LIBRARIES "${CMAKE_SOURCE_DIR}/libs/SDL2/x86/SDL2.lib;${CMAKE_SOURCE_DIR}/libs/SDL2/x86/SDL2main.lib")
	set(SDL2_GFX_LIBRARY "${CMAKE_SOURCE_DIR}/libs/SDL2_gfx/x86/SDL2_gfx.lib")

	file(COPY ${CMAKE_SOURCE_DIR}/libs/SDL2/x86/SDL2.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin/Debug/)
	file(COPY ${CMAKE_SOURCE_DIR}/libs/SDL2_gfx/x86/SDL2_gfx.dll DESTINATION ${CMAKE_SOURCE_DIR}/bin/Debug/)
endif()

find_path(SDL2_INCLUDE_DIRS SDL.h PATHS ${CMAKE_SOURCE_DIR}/include/SDL2)
find_path(SDL2_GFX_INCLUDE SDL2_gfxPrimitives.h PATHS ${CMAKE_SOURCE_DIR}/include/SDL2_gfx)
endif(WIN32)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_GFX_INCLUDE})
target_link_libraries(CarAI ${SDL2_LIBRARIES} ${SDL2_GFX_LIBRARY})
message("Libs linked")

INSTALL(TARGETS CarAI DESTINATION "./bin")

message("Build done")