cmake_minimum_required (VERSION 3.0)
project (vector)

SET(VECTOR_HEADER_FILES MyMallocator.hpp vector.hpp)
SET(VECTOR_SOURCE_FILES main.cpp)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
endif()

add_executable(vector ${VECTOR_HEADER_FILES} ${VECTOR_SOURCE_FILES})