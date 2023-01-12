cmake_minimum_required(VERSION 2.8.9)
project(ProtoPhysics)

# Include libraries
include_directories(include)

# Set Sources
file(GLOB SOURCES "src/*.cpp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ProtoPhysics ${SOURCES})

target_link_libraries(ProtoPhysics pthread glfw GLEW libGLEW.so libGL.so)