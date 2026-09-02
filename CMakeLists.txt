cmake_minimum_required(VERSION 3.31.3)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

project(unitcircle)

file(GLOB SOURCES "./src/*.cpp")

add_executable(unitcircle ${SOURCES})

target_include_directories(unitcircle PUBLIC "./include")

find_package(SDL3 REQUIRED)

target_link_libraries(unitcircle PRIVATE SDL3::SDL3)
