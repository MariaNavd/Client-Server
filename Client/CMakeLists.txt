cmake_minimum_required (VERSION 3.8)

project(Client)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SRC_LIST client.cpp main.cpp)

add_executable(client ${SRC_LIST})
