cmake_minimum_required(VERSION 3.10)

# Project name
project(LearnOpenGL VERSION 1.0)

# Add Static Library
add_executable(LearnOpenGL
               "${CMAKE_SOURCE_DIR}/src/main.cpp"
               "${CMAKE_SOURCE_DIR}/src/glad.c")

target_include_directories(LearnOpenGL
                           PUBLIC
                           "${CMAKE_SOURCE_DIR}/include")

if (WIN32)
  target_link_libraries(LearnOpenGL
                      "${CMAKE_SOURCE_DIR}/lib/win_x64/glfw3.lib")
endif (WIN32)

if (UNIX)
  SET(GCC_COVERAGE_COMPILE_FLAGS "-Wl,--no-as-needed -ldl")
  SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

  target_link_libraries(LearnOpenGL
                        "${CMAKE_SOURCE_DIR}/lib/linux/libglfw3.a")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif (UNIX)


# Make top-level CMakeLists aware of sub-CMakeLists
# add_subdirectory("${CMAKE_SOURCE_DIR}/test")
