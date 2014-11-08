cmake_minimum_required(VERSION 2.8.11)

project(exotica)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
list(APPEND CMAKE_CXX_FLAGS "-std=c++11")

# Find the QtWidgets library
find_package(Qt5Core)

add_library(exotica marshal.cpp Encoder.cpp)

# Use the Core module from Qt 5.
target_link_libraries(exotica Qt5::Core)
