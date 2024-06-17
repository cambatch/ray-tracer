#pragma once

#include <cstdint>
#include <GLFW/glfw3.h>

inline uint32_t colorTexture;
inline GLFWwindow* window;


// Create vbo, vbo, ibo. Fill vbo and ibo buffers with data. Create shaders.
void InitDrawing(uint32_t screenWidth, uint32_t screenHeight);
void TerminateDrawing();

// Update texture with color data from buffer
void UpdateTexture(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t* const buffer);

// Draw the quad with the colorTexture to the window.
void Draw();
