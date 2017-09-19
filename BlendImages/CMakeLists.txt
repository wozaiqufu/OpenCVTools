cmake_minimum_required(VERSION 3.8)
project(AddingTrackBar)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)
set(SOURCE_FILES main.cpp)

add_executable(AddingTrackBar ${SOURCE_FILES})
target_link_libraries(AddingTrackBar ${OpenCV_LIBS})