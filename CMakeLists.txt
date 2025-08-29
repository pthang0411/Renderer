cmake_minimum_required(VERSION 3.10)
project(Renderer)
add_executable(hello src/main.cpp)
include_directories(
    third-party/errorReporting
    third-party/glad/include
    third-party/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/include/GLFW
    third-party/glm
    third-party/ShaderHandler
    third-party/stb_image
    renderer
)
add_library(glad third-party/glad/src/glad.c)
add_library(stbi third-party/stb_image/stb_image.cpp)
link_libraries(
    glad 
    ${CMAKE_SOURCE_DIR}/third-party/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a
    stbi
)
target_link_libraries(hello glad stbi ${CMAKE_SOURCE_DIR}/third-party/glfw-3.4.bin.WIN64/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3.a)