cmake_minimum_required(VERSION 3.10)

project(smoothing)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenCV REQUIRED)

add_executable(app src/main.cpp src/spatial_filters.cpp)
target_include_directories(app PRIVATE include)

target_link_libraries(app ${OpenCV_LIBS})

# add_executable(filtro aplicacao_filtro.cpp)
# target_link_libraries(filtro ${OpenCV_LIBS})


