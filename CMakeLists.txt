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
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(ZMQ REQUIRED libzmq)
  if(ZMQ_FOUND)
    add_definitions(-DENABLE_ZMQ)
    include_directories(${ZMQ_INCLUDE_DIRS})
    link_directories(${ZMQ_LIBRARY_DIRS})
    set(ZEROMQ_LIBRARIES ${ZMQ_LIBRARIES})  # Set this for the subdirectories
    message(STATUS "ZeroMQ found via pkg-config: ${ZMQ_LIBRARIES}")
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

include_directories(uuagent/hdr)

add_subdirectory(uuagent)

install(
  PROGRAMS
  ${CMAKE_BINARY_DIR}/uuagent/uuagent
  DESTINATION /usr/local/bin
)
