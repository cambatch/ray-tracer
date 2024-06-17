#include "glDraw.hpp"


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>


static uint32_t vao;
static uint32_t vbo;
static uint32_t ibo;
static uint32_t shaderProgram;

static std::vector<float> quadVertices = {
    // Bottom left
    -1.0f, -1.0f,   0.0f, 0.0f,
    // Top left
    -1.0f, 1.0f,    0.0f, 1.0f,
    // Bottom right
     1.0f, -1.0f,   1.0f, 0.0f,
     // Top right
     1.0f, 1.0f,    1.0f, 1.0f
};

static std::vector<uint32_t> quadIndices = { 0, 1, 2, 2, 1, 3 };


static void InitGLFW(uint32_t width, uint32_t height) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        exit(1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window = glfwCreateWindow(width, height, "ray-tracer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window\n";
        exit(1);
    }
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);
    int version = gladLoadGL(glfwGetProcAddress);
    if (!version) {
        std::cerr << "GLAD failed to initialize\n";
        exit(1);
    }
}

static std::string ReadShaderFile(const char* path) {
    std::ifstream file(path);
    std::stringstream ss;

    if (!file.is_open()) {
        std::cout << "Failed to openg file: " << path << "!\n";
        exit(1);
    }

    ss << file.rdbuf();
    return ss.str();
}

static bool ShaderCompileStatus(uint32_t id) {
    int res;
    glGetShaderiv(id, GL_COMPILE_STATUS, &res);
    if (!res) {
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &res);
        char* message = new char[res];
        glGetShaderInfoLog(id, res, &res, message);
        std::cerr << "Failed to compile shader. " << message << '\n';
        delete[] message;
        return false;
    }

    return true;
}

static uint32_t CreateEmptyTexture(uint32_t width, uint32_t height) {
    uint32_t id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    return id;
}

void InitDrawing(uint32_t width, uint32_t height) {
    InitGLFW(width, height);
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * quadIndices.size(), quadIndices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);


    std::string vertSource = ReadShaderFile("assets/shaders/quad.vert");
    std::string fragSource = ReadShaderFile("assets/shaders/quad.frag");
    const char* vertSrc = vertSource.c_str();
    const char* fragSrc = fragSource.c_str();

    uint32_t vertShader;
    uint32_t fragShader;

    vertShader = glCreateShader(GL_VERTEX_SHADER);
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &vertSrc, nullptr);
    glCompileShader(vertShader);
    if (!ShaderCompileStatus(vertShader)) {
        exit(1);
    }
    glShaderSource(fragShader, 1, &fragSrc, nullptr);
    glCompileShader(fragShader);
    if (!ShaderCompileStatus(fragShader)) {
        exit(1);
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "uTexture"), 0);

    colorTexture = CreateEmptyTexture(width, height);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void UpdateTexture(uint32_t x, uint32_t y, uint32_t width, uint32_t height, const uint8_t* const buffer) {
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer + y * width * 3);
}

void TerminateDrawing() {
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ibo);

    glDeleteTextures(1, &colorTexture);

    glDeleteProgram(shaderProgram);
}

void Draw() {
    glfwPollEvents();
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, colorTexture);
    
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, quadIndices.size(), GL_UNSIGNED_INT, 0);

    glfwSwapBuffers(window);
}
