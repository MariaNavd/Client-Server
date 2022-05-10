cmake_minimum_required (VERSION 3.8)

project(Client-Server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(Server)
add_subdirectory(Client)
