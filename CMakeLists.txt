cmake_minimum_required(VERSION 3.31.3)

set(CMAKE_CXX_STANDARD 17)

project(proj)

add_executable(proj "./src/main.cpp" "./src/util.cpp")

target_include_directories(proj PUBLIC "./include")

find_package(SDL3 REQUIRED)

target_link_libraries(proj PRIVATE SDL3::SDL3)