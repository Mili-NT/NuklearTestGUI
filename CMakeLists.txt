cmake_minimum_required(VERSION 3.24)
project(NuklearTestGUI C)

set(CMAKE_C_STANDARD 11)

# Add GLFW and OpenGL libraries
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(NuklearTestGUI main.c)
target_link_libraries(NuklearTestGUI glfw GLEW::GLEW ssl crypto OpenGL::GL)
