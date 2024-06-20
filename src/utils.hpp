#pragma once

#include <limits>
#include <numbers>
#include <random>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>


constexpr double PI = std::numbers::pi;
constexpr double PI_2 = std::numbers::pi / 2;
constexpr double INF = std::numeric_limits<double>::infinity();


inline double RandomDouble() {
    static std::mt19937 gen(0);
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

inline float RandomFloat() {
    static std::mt19937 gen(0);
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

