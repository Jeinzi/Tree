cmake_minimum_required(VERSION 2.6)
project(Tree)

set(CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "-std=c++11")

file(GLOB SRCS "Progresscounter/*.cpp" "Progresscouner/*.h" "Tree/*.cpp" "Tree/*.h" "Utility/*.cpp" "Utility/*.h")
add_executable(Tree ${SRCS})
