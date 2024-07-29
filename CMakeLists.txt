cmake_minimum_required(VERSION 3.18)
project(MyProject)

# Set compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O2")

# Find SFML package
find_package(SFML COMPONENTS graphics system window REQUIRED)
find_package(TGUI 1 REQUIRED)

# Gather source files
file(GLOB SOURCES 
    "src/*.cpp"
    "src/misc/*.cpp"
    "src/utils/*.cpp"
    "src/physics/*.cpp"
    "src/app/*.cpp"
    "src/state/*/*.cpp"
)

# Create executable target
add_executable(main ${SOURCES})

# Link necessary libraries
target_link_libraries(main 
    sfml-graphics 
    sfml-system 
    sfml-window
    TGUI::TGUI
)