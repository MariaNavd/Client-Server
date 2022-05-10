cmake_minimum_required (VERSION 3.8)

project(Data)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_LIB data.cpp)

add_library(data STATIC ${SOURCE_LIB})
