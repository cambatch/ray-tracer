cmake_minimum_required(VERSION 3.25)
project(imgui)

set(IMGUI_SOURCE
    imgui.h
    imgui.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
)

set(IMGUI_BACKEND
    backends/imgui_impl_glfw.h
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_opengl3.cpp
)


add_library(imgui STATIC ${IMGUI_SOURCE} ${IMGUI_BACKEND})
target_link_libraries(imgui glfw)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} backends)
