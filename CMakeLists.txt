cmake_minimum_required(VERSION 2.8.0)
	project(Game)

	set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/bin)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
	set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

	set(CMAKE_BUILD_TYPE Debug)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

	include_directories(src)
	include_directories(/usr/include)
	file(GLOB SOURCES "src/*.cpp")

	set(PROJECT_LINK_LIBS sdl2)

	add_executable(game ${SOURCES})
	target_link_libraries(game sdl2)
	target_link_libraries(game SDL2)
