set(CORE_SOURCES
    src/intensity_transformations.cpp
    src/interpolations.cpp
    src/equalizations.cpp
    src/spatial_filters.cpp
    src/limiars.cpp
)

add_library(dip_core ${CORE_SOURCES})

target_include_directories(dip_core PUBLIC include)

target_link_libraries(dip_core PUBLIC ${OpenCV_LIBS})
