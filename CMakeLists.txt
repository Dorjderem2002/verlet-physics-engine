cmake_minimum_required(VERSION 3.18)
project(MyProject)

# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# Find SFML package
find_package(SFML COMPONENTS graphics system window REQUIRED)

include_directories(
    ${SFML_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/misc
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/physics
    ${CMAKE_CURRENT_SOURCE_DIR}/src/physics/*
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app
    ${CMAKE_CURRENT_SOURCE_DIR}/src/state/*
)

# Gather source files
file(GLOB SOURCES 
    "src/*.cpp"
    "src/misc/*.cpp"
    "src/utils/*.cpp"
    "src/physics/*.cpp"
    "src/physics/*/*.cpp"
    "src/app/*.cpp"
    "src/app/state/*/*.cpp"
)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resource DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Create executable target
add_executable(main ${SOURCES})

# Link necessary libraries
target_link_libraries(main 
    sfml-graphics 
    sfml-system 
    sfml-window
)