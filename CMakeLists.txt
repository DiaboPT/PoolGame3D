cmake_minimum_required(VERSION 3.12)
project(PROJECT_NAME)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/header)
include_directories(${PROJECT_SOURCE_DIR}/header/opt)

# Add executable
add_executable(PROJECT_NAME Source.cpp)

# Link libraries
target_link_libraries(PROJECT_NAME
    ${OPENGL_LIBRARIES}
    GLEW::GLEW
    glfw
)