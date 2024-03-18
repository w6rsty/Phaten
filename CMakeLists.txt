cmake_minimum_required(VERSION 3.20)

project(Phaten LANGUAGES CXX C)

include(CMake/Tools.cmake)

add_subdirectory(3rdlibs)
add_subdirectory(Phaten)
add_subdirectory(PhatenTest)