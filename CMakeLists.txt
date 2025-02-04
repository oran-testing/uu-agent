# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

project(Uu-Interface-Agent
  VERSION 1.0
  DESCRIPTION "User to User interface agent"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(ENABLE_ZEROMQ         "Enable ZeroMQ"                            ON)
option(ENABLE_UHD            "Enable UHD"                               ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

set(RF_FOUND false)

if(ENABLE_UHD)
  find_package(UHD)
  if(UHD_FOUND)
    set(RF_FOUND true)
    include_directories(${UHD_INCLUDE_DIRS})
    link_directories(${UHD_LIBRARY_DIRS})
    message(STATUS "UHD library found")
  else()
    message(STATUS "UHD library not found")
  endif()
endif()

if(ENABLE_ZEROMQ)
  find_package(ZeroMQ)
  if(ZEROMQ_FOUND)
    set(RF_FOUND true)
    include_directories(${ZEROMQ_INCLUDE_DIRS})
    link_directories(${ZEROMQ_LIBRARY_DIRS})
  endif()
endif()

if(RF_FOUND)
  message(STATUS "RF device found")
else()
  message(FATAL_ERROR "no RF device found")
endif()

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

find_package(Boost REQUIRED COMPONENTS program_options)

include_directories(lib/include)

add_subdirectory(lib)
add_subdirectory(uuagent)
