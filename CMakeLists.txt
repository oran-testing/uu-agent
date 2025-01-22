# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

project(uuagent
  VERSION 1.0
  DESCRIPTION "User to User interface agent"
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(UHD REQUIRED)

if(UHD_FOUND)
  message(STATUS "UHD library found")
else()
  message(FATAL_ERROR "UHD library not found")
endif()

message(STATUS "UHD include dirs: ${UHD_INCLUDE_DIRS}")
message(STATUS "UHD libraries: ${UHD_LIBRARIES}")

include_directories(${PROJECT_SOURCE_DIR}/hdr)
file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cc)

find_package(yaml-cpp REQUIRED)
include_directories(${YAML_CPP_INCLUDE_DIRS})

include_directories(${UHD_INCLUDE_DIRS})
link_directories(${UHD_LIBRARY_DIRS})

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME} ${UHD_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${YAML_CPP_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/local/bin)
