cmake_minimum_required (VERSION 2.6)
project (MATLAB-Dataflow)

# Headers
include_directories(include)

# Source
add_executable(TestFlow test/testFlowMP.cpp)

# Find dependencies
#find_package(Boost COMPONENTS thread system REQUIRED)
find_package(Boost 1.54.0 REQUIRED COMPONENTS system filesystem thread)
target_link_libraries(TestFlow
  ${Boost_THREAD_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
)
#if(Boost_FOUND)
#    include_directories(${Boost_INCLUDE_DIRS}) 
#    message(${Boost_INCLUDE_DIRS})
#endif()

set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall -lboost_thread -lboost_system -pthread")

