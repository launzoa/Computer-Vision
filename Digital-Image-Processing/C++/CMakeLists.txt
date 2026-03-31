cmake_minimum_required(VERSION 3.10)
project(Digital-Image-Processing)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(OpenCV REQUIRED)

add_subdirectory(core)

add_subdirectory(programs)

