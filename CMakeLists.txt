cmake_minimum_required(VERSION 3.10)
project(rootPlotter)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find ROOT package
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# Create shared library
add_library(rootPlotter SHARED
    rootPlotter.cpp
    rootPlotter.h
)

# Link ROOT libraries to our shared library
target_link_libraries(rootPlotter PUBLIC ${ROOT_LIBRARIES})

# Include directories for the library
target_include_directories(rootPlotter PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${ROOT_INCLUDE_DIRS}
)

# Create executable
add_executable(ColorPaletteDemo Demonstrations/ColorPaletteDemo.cpp)
add_executable(ObjectTypeDemo Demonstrations/ObjectTypeDemo.cpp)

# Link the executable with our library and ROOT libraries
target_link_libraries(ColorPaletteDemo
    PRIVATE
    rootPlotter
    ${ROOT_LIBRARIES}
)

target_link_libraries(ObjectTypeDemo
    PRIVATE
    rootPlotter
    ${ROOT_LIBRARIES}
)
