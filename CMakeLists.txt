cmake_minimum_required(VERSION 3.6)
project(spiess)

set(CMAKE_CXX_STANDARD 14)

add_executable(spiess main.cpp solver.cpp graph.cpp)
target_link_libraries(spiess -std=c++14)

install(TARGETS spiess DESTINATION bin)

