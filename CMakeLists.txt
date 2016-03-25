cmake_minimum_required (VERSION 2.6)

project(MATLAB-Dataflow)

#subdirs(src)
#add_subdirectory(src)

# Headers
include_directories(include)

# Allows for wildcard additions:
#file(GLOB SOURCES "src/*.cpp")
set(SOURCES src/flowMP.cpp src/graph.cpp)

# Additional Source
#link_directories(${MATLAB-Dataflow_SOURCE_DIR}/src)

# Source
#add_executable(TestFlow test/testFlowMP.cpp)
add_executable(Benchmark src/mainPD_bm.cpp;${SOURCES})

# Find dependencies
find_package(Boost 1.54.0 REQUIRED COMPONENTS system filesystem timer thread)
#target_link_libraries(TestFlow
#  ${Boost_THREAD_LIBRARY}
#  ${Boost_FILESYSTEM_LIBRARY}
#  ${Boost_SYSTEM_LIBRARY}
#  ${Boost_TIMER_LIBRARY}
#)

target_link_libraries(Benchmark
  ${Boost_THREAD_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${Boost_TIMER_LIBRARY}
)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -lboost_thread -lboost_system -pthread")
