#pragma once

#include <cstdint>
#include <glm/glm.hpp>


using Colorf = glm::vec3;

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

inline Color ConvertColorToInt(const Colorf& c) {
    int r = 255.999 * c.x;
    int g = 255.999 * c.y;
    int b = 255.999 * c.z;
    return Color(r, g, b);
}
