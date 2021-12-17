project (wave)
site_name (www.ostis.net)
cmake_minimum_required (VERSION 2.6)

set(SC_MACHINE_ROOT "${CMAKE_SOURCE_DIR}/../sc-machine/")
set(SCP_MACHINE_ROOT "${CMAKE_SOURCE_DIR}")

set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SC_MACHINE_ROOT}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SC_MACHINE_ROOT}/bin)
set( CMAKE_CXX_FLAGS "-std=c++11")

link_directories(${SC_MACHINE_ROOT}/bin)

set(SC_MEMORY_SRC "${SC_MACHINE_ROOT}/sc-memory/")

if (${CMAKE_SYSTEM_NAME} STREQUAL Windows)
	if(MSVC)
		message(STATUS "Compiler: MSVC, version: " ${MSVC_VERSION})
	endif(MSVC)
	if(BORLAND)
		message(STATUS "Compiler: BCC")
	endif(BORLAND)
	else(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
		message(STATUS "Only GCC is supported on Linux")
	endif(${CMAKE_SYSTEM_NAME} STREQUAL Windows)

message(STATUS "CMake generates " ${CMAKE_GENERATOR})

include_directories(${SC_MEMORY_SRC} ${GLIB2_INCLUDE_DIRS}${SC_MACHINE_ROOT})
add_executable(wave wavefindpath.cpp utils.cpp utils.h)


target_link_libraries(wave sc-core sc-memory ${GLIB2_LIBRARIES})
