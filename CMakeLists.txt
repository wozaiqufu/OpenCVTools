cmake_minimum_required(VERSION 3.8)
project(cvRootProject)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXCUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
include_directories("${PROJECT_INCLUDE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
add_subdirectory(BlendImages)

