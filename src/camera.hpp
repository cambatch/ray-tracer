#pragma once

#include <cstdint>

#include "utils.hpp"
#include "Ray.hpp"

inline glm::vec3 SampleSquare() {
    return { (float)RandomDouble() - 0.5f, (float)RandomDouble() - 0.5f, 0};
}


struct Camera {
    float AspectRatio;
    glm::vec3 Pixel100Loc;
    glm::vec3 CamCenter;
    glm::vec3 PixelDeltaU;
    glm::vec3 PixelDeltaV;

    Camera(const glm::vec3& center, uint32_t width, uint32_t height) {
        CamCenter = center;
        AspectRatio = (float)width / height;

        float focalLength = 1.0;
        float viewportHeight = 2.0;
        float viewportWidth = viewportHeight * AspectRatio;
        glm::vec3 viewportU = { viewportWidth, 0, 0 };
        // Rendering to an image viewportHeight should be negative. It is positive in this case for opengl textures.
        glm::vec3 viewportV = { 0, viewportHeight, 0 };

        PixelDeltaU = viewportU / (float)width;
        PixelDeltaV = viewportV / (float)height;

        glm::vec3 viewportUpperLeft = CamCenter - glm::vec3(0, 0, focalLength) - viewportU/2.f - viewportV/2.f;
        Pixel100Loc = viewportUpperLeft + .5f * (PixelDeltaU + PixelDeltaV);
    }

    Ray GetRay(uint32_t x, uint32_t y) {
        glm::vec3 offset = SampleSquare();
        glm::vec3 pixelSample = Pixel100Loc + ((x + offset.x) * PixelDeltaU) + ((y + offset.y) * PixelDeltaV);
        glm::vec3 rayOrigin = CamCenter;
        glm::vec3 rayDir = pixelSample - rayOrigin;
        return { rayOrigin, rayDir };
    }
};
