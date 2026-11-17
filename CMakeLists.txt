cmake_minimum_required(VERSION 3.23)
project(web-server LANGUAGES CXX)

if(NOT UNIX)
    message(FATAL_ERROR "Project only supports UNIX like operating systems")
endif()

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O2 -Wall -Wextra -Isrc/ -MMD -MP -fcommon)

add_subdirectory(src)

message(STATUS "Build configuration: " ${CMAKE_BUILD_TYPE})
