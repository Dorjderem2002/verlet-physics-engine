cmake_minimum_required(VERSION 3.12)
project(MyProject)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.6.1 COMPONENTS graphics system window REQUIRED)

file(GLOB SOURCES "*.cpp" "physics/*.cpp")

add_executable(main ${SOURCES})

target_link_libraries(main PRIVATE sfml-graphics sfml-system sfml-window)