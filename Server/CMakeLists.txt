cmake_minimum_required (VERSION 3.8)

project(Server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_LIST server.cpp main.cpp)

include_directories(Data)

add_executable(server ${SRC_LIST})

add_subdirectory(Data)

target_link_libraries(server data)
