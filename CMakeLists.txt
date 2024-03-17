cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME Phaten)
set(CMAKE_CXX_STANDARD 17)

project(${PROJECT_NAME} LANGUAGES CXX C)

include(CMake/Tools.cmake)

add_subdirectory(3rdlibs)
add_subdirectory(Phaten)