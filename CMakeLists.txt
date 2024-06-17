cmake_minimum_required(VERSION 3.25)
project(ray-tracer)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(dep/glfw-3.4)

add_subdirectory(dep/glad)

add_subdirectory(dep/imgui)

add_subdirectory(dep/glm-light/glm)

add_subdirectory(dep/stb)


set(SOURCES
    src/main.cpp
    src/glDraw.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE dep/glm-light)
target_link_libraries(${PROJECT_NAME} glfw glad imgui glm stb)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)
