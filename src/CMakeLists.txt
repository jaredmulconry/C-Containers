cmake_minimum_required (VERSION 3.1)
project (containers)

SET(CONTAINER_HEADER_FILES MyMallocator.hpp vector.hpp dense_map.hpp)
SET(CONTAINER_SOURCE_FILES main.cpp)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	# using Clang
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	# using GCC
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pedantic")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
	# using Intel C++
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	# using Visual Studio C++
	# message(AUTHOR_WARNING "Requires VS2015 or later")
	if(MSVC_VERSION LESS 1900)
		MESSAGE(FATAL_ERROR "Project requires VS2015 or later")
	endif()
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /sdl")
else()
	# unknown c++ compiler in use
	message(FATAL_ERROR "Unknown c++ compiler target in use.")
endif()

add_executable(containers ${CONTAINER_HEADER_FILES} ${CONTAINER_SOURCE_FILES})
install(TARGETS containers DESTINATION "build")