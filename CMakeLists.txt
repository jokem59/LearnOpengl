cmake_minimum_required(VERSION 3.10)

set(NAME "LearnOpenGL")

# Project name
project(${NAME} VERSION 1.0)

# Add Executable Library
add_executable(${NAME}
  "${CMAKE_SOURCE_DIR}/src/main.cpp"
  "${CMAKE_SOURCE_DIR}/src/stb_image.cpp"
  "${CMAKE_SOURCE_DIR}/src/glad.c")

include_directories(${NAME}
  PUBLIC
  "${CMAKE_SOURCE_DIR}/include")

configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)

if (WIN32)
  target_link_libraries(${NAME}
                        "${CMAKE_SOURCE_DIR}/lib/win_x64/glfw3.lib")
endif (WIN32)
if (UNIX)
  set(GCC_COVERAGE_COMPILE_FLAGS "-Wl,--no-as-needed -ldl")
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

  target_link_libraries(${NAME}
    "${CMAKE_SOURCE_DIR}/lib/linux/libglfw3.a")

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)


# Make top-level CMakeLists aware of sub-CMakeLists
# add_subdirectory("${CMAKE_SOURCE_DIR}/test")
