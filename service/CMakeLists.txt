cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

# Specify project name
project(Ara)

# Specify the source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Specify the include directories
include_directories(include)

# Create the executable
add_executable(main ${SOURCES})
