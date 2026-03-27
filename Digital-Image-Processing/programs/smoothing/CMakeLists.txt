add_executable(app_colors main.cpp)
add_executable(app_interpolationns main.cpp)
add_executable(app_equalizations main.cpp)
add_executable(app_smoothing main.cpp)

target_link_libraries(app_smoothing dip_core)
