cmake_minimum_required(VERSION 3.5)
project(IntervalTree)

set(CMAKE_CXX_STANDARD 17)
set(MAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_BUILD_TYPE Release)
# set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# # you may need these 2 lines, and set your own paths
set(PYTHON_EXECUTABLE "C:\\Users\\stamr\\AppData\\Local\\Programs\\Python\\Python37\\python.exe")
set(PYTHON_INCLUDE_DIRS "C:\\Users\\stamr\\AppData\\Local\\Programs\\Python\\Python37\\include\\Python.h")

add_subdirectory(extern/pybind11)

pybind11_add_module(intervalTree IntervalTree.cpp bind.cpp)