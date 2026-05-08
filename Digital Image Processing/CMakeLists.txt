cmake_minimum_required(VERSION 3.14)
project(DIP LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 1. Avisa o CMake onde estão as ferramentas de Desktop (Linux) ANTES de procurar o Qt
set(QT_HOST_PATH "/home/launzoa/dev/Qt/6.11.0/gcc_64" CACHE PATH "Qt Host" FORCE)

# 2. Força o uso do Qt WebAssembly para a compilação
set(Qt6_DIR "/home/launzoa/dev/Qt/6.11.0/wasm_singlethread/lib/cmake/Qt6" CACHE PATH "Qt WASM" FORCE)

# 3. Agora sim, carregamos o Qt de forma segura
find_package(Qt6 COMPONENTS Widgets REQUIRED)

# 4. Apontamento forçado para a versão Web do OpenCV
set(OpenCV_DIR "/home/launzoa/dev/opencv/build_wasm" CACHE PATH "OpenCV WASM" FORCE)
find_package(OpenCV REQUIRED)

qt_add_executable(DIP
    main.cpp
    mainwindow.cpp
    mainwindow.h
    src/operations.cpp
    src/operations.h
)

target_link_libraries(DIP PRIVATE
    Qt6::Widgets
    ${OpenCV_LIBS}
)
