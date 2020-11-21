cmake_minimum_required(VERSION 3.10)

# Project name
project(LearnOpenGL VERSION 1.0)

# Add Static Library
add_executable(LearnOpenGL
               "${CMAKE_SOURCE_DIR}/src/main.cpp"
               "${CMAKE_SOURCE_DIR}/src/glad.c")

target_link_libraries(LearnOpenGL
                      "${CMAKE_SOURCE_DIR}/lib/glfw3.lib")

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_include_directories(LearnOpenGL
                           PUBLIC
                           "${CMAKE_SOURCE_DIR}/include")


# Make top-level CMakeLists aware of sub-CMakeLists
# add_subdirectory("${CMAKE_SOURCE_DIR}/test")
