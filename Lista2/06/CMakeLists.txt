cmake_minimum_required(VERSION 3.20)

project(ProcessamentoGrafico)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# GLAD
add_library(glad STATIC
    glad/src/gl.c
)

target_include_directories(glad PUBLIC
    glad/include
)

# Programa
add_executable(ProcessamentoGrafico
    src/main.cpp
)

target_link_libraries(ProcessamentoGrafico
    PRIVATE
    glfw
    glad
    glm::glm 
    opengl32
)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
)

FetchContent_MakeAvailable(glm)