#pragma once

#include <cstdint>
#include <glm/glm.hpp>


using Colorf = glm::vec3;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};


inline float LinearToGamma(float linearComp) {
    if (linearComp > 0) {
        return std::sqrtf(linearComp);
    } else {
        return 0;
    }
}

inline Color ConvertColorToInt(const Colorf& c) {
    // Convert to gamma space
    float gr = LinearToGamma(c.x);
    float gg = LinearToGamma(c.y);
    float gb = LinearToGamma(c.z);

    int r = 255.999 * gr;
    int g = 255.999 * gg;
    int b = 255.999 * gb;
    return Color(r, g, b);
}

