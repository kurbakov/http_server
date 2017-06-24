cmake_minimum_required (VERSION 2.8)
project (final)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -o3")

set (SOURCE_FILES src/final.cpp src/http_server.cpp)
add_executable (final ${SOURCE_FILES})