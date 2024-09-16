# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(uuagent VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set include directories for header files
include_directories(${PROJECT_SOURCE_DIR}/hdr)

# Add source files
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)

# Add the executable target
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Optionally specify compile options, link libraries, etc.
# target_link_libraries(${PROJECT_NAME} <LIBRARY>)
