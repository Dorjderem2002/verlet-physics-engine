cmake_minimum_required(VERSION 3.18)
project(MyProject)

# Set compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find packages
find_package(SFML COMPONENTS graphics system window REQUIRED)

# Include all source files from the current directory and the "physics" subdirectory
file(GLOB SOURCES "src/*.cpp" "src/physics/*.cpp" "src/app/*.cpp")

# Add executable target
add_executable(main ${SOURCES})

# Link libraries
target_link_libraries(main sfml-graphics sfml-system sfml-window)