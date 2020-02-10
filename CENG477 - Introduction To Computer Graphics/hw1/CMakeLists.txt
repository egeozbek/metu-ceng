cmake_minimum_required(VERSION 3.13)  # CMake version check
project(graph1)               # Create project "simple_example"
set(CMAKE_CXX_STANDARD 11)            # Enable c++14 standard

# Add main.cpp file of project root directory as source file

set(SOURCE_FILES main.cpp Light.cpp Camera.cpp Image.cpp Light.cpp Material.cpp Ray.cpp Scene.cpp Shape.cpp tinyxml2.cpp VectorUtils.cpp)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(simple_example ${SOURCE_FILES})