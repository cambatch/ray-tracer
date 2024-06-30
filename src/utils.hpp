#pragma once

#include <limits>
#include <numbers>
#include <random>
#include <cstring>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>


constexpr double PI = std::numbers::pi;
constexpr double PI_2 = std::numbers::pi / 2;
constexpr double INF = std::numeric_limits<double>::infinity();

// Flip buffer containing opengl texture to support image formats.
inline void FlipBufferVertical(std::vector<uint8_t>& buffer, uint32_t width, uint32_t height) {
    uint32_t rowSize = width * 3;
    std::vector<uint8_t> tempRow(rowSize);

    for (uint32_t y = 0; y < height / 2; ++y) {
        uint32_t topRow = y * rowSize;
        uint32_t bottomRow = (height - y - 1) * rowSize;

        std::memcpy(tempRow.data(), &buffer[topRow], rowSize);
        std::memcpy(&buffer[topRow], &buffer[bottomRow], rowSize);
        std::memcpy(&buffer[bottomRow], tempRow.data(), rowSize);
    }
}

inline double RandomDouble() {
    static std::mt19937 gen;
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

inline float RandomFloat() {
    static std::mt19937 gen;
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble();
}

inline double RandomFloat(double min, double max) {
    return min + (max - min) * RandomFloat();
}

inline glm::vec3 RandomVec3() {
    return glm::vec3(RandomFloat(), RandomFloat(), RandomFloat());
}

inline glm::vec3 RandomVec3(float min, float max) {
    return glm::vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
}

inline glm::vec3 RandomInUnitSphere() {
    while (true) {
        auto p = RandomVec3(-1, 1);
        if (glm::length2(p) < 1) {
            return p;
        }
    }
}

inline glm::vec3 RandomInUnitDisk() {
    while(true) {
        auto p = glm::vec3(RandomFloat(-1, 1), RandomFloat(-1, 1), 0);
        if (glm::length2(p) < 1) {
            return p;
        }
    }
}

inline glm::vec3 RandomUnitVec3() {
    return glm::normalize(RandomInUnitSphere());
}

inline glm::vec3 RandomOnHemisphere(const glm::vec3& normal) {
    glm::vec3 onUnitSphere = RandomUnitVec3();
    if (glm::dot(onUnitSphere, normal) > 0.0) {
        return onUnitSphere;
    } else {
        return -onUnitSphere;
    }
}

inline bool NearZero(const glm::vec3& v) {
    float s = 1e-8;
    return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
}

