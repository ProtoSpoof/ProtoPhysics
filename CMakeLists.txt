cmake_minimum_required(VERSION 3.22.1)
project(ProtoPhysics)

# Include libraries
include_directories(include)

# Set Sources
file(GLOB SOURCES "src/*.[ch]pp")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ProtoPhysics ${SOURCES})

if(CMAKE_HOST_UNIX)
    target_link_libraries(ProtoPhysics pthread glfw GLEW libGLEW.so libGL.so)
else()
    target_link_libraries(ProtoPhysics mingw32 glew32 glu32 opengl32 glfw3 gdi32 -static-libgcc -static-libstdc++)
endif()
