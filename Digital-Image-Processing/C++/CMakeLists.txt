cmake_minimum_required(VERSION 3.14)
project(ImageFiltering LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(OpenCV REQUIRED)

add_executable(ImageFiltering
    main.cpp
    mainwindow.cpp
    mainwindow.h
    src/operations.cpp
    src/operations.h
)

target_link_libraries(ImageFiltering PRIVATE
    Qt6::Widgets
    ${OpenCV_LIBS}
)
