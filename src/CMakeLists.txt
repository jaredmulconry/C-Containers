cmake_minimum_required (VERSION 3.1)
project (containers)

include(CTest)

include_directories(${CMAKE_CURRENT_LIST_DIR})
get_filename_component(SOURCE_DIRECTORY_PARENT "${CMAKE_SOURCE_DIR}" DIRECTORY)
SET(MY_OWN_INSTALL_PREFIX "${SOURCE_DIRECTORY_PARENT}" CACHE PATH "Prefix prepended to install directories")
SET(CMAKE_INSTALL_PREFIX "${MY_OWN_INSTALL_PREFIX}" CACHE INTERNAL "Prefix prepended to install directories" FORCE)

add_executable(vector_test MyMallocator.hpp vector.hpp test/vector_test.cpp)
add_executable(dense_map_test MyMallocator.hpp dense_map.hpp test/dense_map_test.cpp)
add_executable(dense_map_perf MyMallocator.hpp dense_map.hpp test/randomised_input.hpp test/dense_map_perf.cpp)
install(TARGETS vector_test dense_map_test DESTINATION "build")

add_test(vectorTest vector_test)
add_test(denseMapTest dense_map_test)
add_test(denseMapPerf dense_map_perf)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
	# using Clang
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
	# using GCC
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pedantic -march=native")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -O3")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Intel)
	# using Intel C++
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -ipo")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
	# using Visual Studio C++
	# message(AUTHOR_WARNING "Requires VS2015 or later")
	if(MSVC_VERSION LESS 1900)
		MESSAGE(FATAL_ERROR "Project requires VS2015 or later")
	endif()
	add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /sdl /MP")
	SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Oi /GL")
	set_target_properties(vector_test dense_map_test dense_map_perf PROPERTIES LINK_FLAGS_RELEASE "/LTCG")
else()
	# unknown c++ compiler in use
	message(FATAL_ERROR "Unknown c++ compiler target in use.")
endif()
