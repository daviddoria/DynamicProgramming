cmake_minimum_required(VERSION 2.6)

PROJECT(DynamicProgramming)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")

include_directories(/media/portable/src/Eigen)

if(NOT TARGET Helpers)
  add_subdirectory(Helpers)
endif()

ADD_EXECUTABLE(DynamicProgramming Example.cpp)
target_link_libraries(DynamicProgramming Helpers)
