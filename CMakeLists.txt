cmake_minimum_required(VERSION 2.6)

PROJECT(DynamicProgramming)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++0x")

include_directories(/media/portable/src/Eigen)

add_subdirectory(Helpers)

ADD_EXECUTABLE(DynamicProgramming Example.cpp DynamicProgramming.cpp)
target_link_libraries(DynamicProgramming Helpers)
