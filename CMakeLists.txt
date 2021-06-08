cmake_minimum_required(VERSION 3.17)
project(cpp_path_planner)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp_path_planner src/main.cpp src/Environment.cpp src/Environment.h src/PathPlanner.cpp src/PathPlanner.h src/GraphGenerator.cpp src/GraphGenerator.h)
