#
# Project settings and generic definitions.
#
cmake_minimum_required(VERSION 3.4)
project(hhcp CXX)

# Minimal definitions
add_definitions(
   -Wall 
   -Wextra 
   -std=c++17 
   
   -march=native 
   -mtune=native  
   
   -fopenmp 
   -fgraphite
   -flto
)

# Debug and Release definitions.
# To set the build mode, specify the value of var `CMAKE_BUILD_TYPE` when
# running CMake.
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fno-omit-frame-pointer") # -fsanitize=address

# Fallback to Debug build type automatically
# if no compilation mode was specified.
if(NOT CMAKE_BUILD_TYPE)
   set(CMAKE_BUILD_TYPE "Debug")
endif()

message("-- Using compilation flags of mode \"" ${CMAKE_BUILD_TYPE} "\"")

#
# Project compilation settings.
#

include(cplex.cmake)
include_directories(src)

add_executable(exactMip
   src/mainExactMip.cpp
   src/Instance.cpp
   src/MipModel.cpp
   src/brkga/Solution.cpp
   src/brkga/SortingDecoder.cpp
   src/brkga/SimpleBrkgaSolver.cpp
   src/brkga/brkgaAPI/Population.cpp
   src/brkga/Task.cpp
)
target_link_libraries(exactMip ${CPLEX_LIBRARIES} -flto)
