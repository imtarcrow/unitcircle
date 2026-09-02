cmake_minimum_required(VERSION 3.31.3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

project(proj)

file(GLOB SOURCES "./src/*.cpp")

add_executable(proj ${SOURCES})

target_include_directories(proj PUBLIC "./include")

find_package(SDL3 REQUIRED)

target_link_libraries(proj PRIVATE SDL3::SDL3)
