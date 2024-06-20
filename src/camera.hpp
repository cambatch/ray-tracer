#pragma once

#include <cstdint>

#include "utils.hpp"
#include "ray.hpp"

inline glm::vec3 SampleSquare() {
    return { RandomFloat() - 0.5f, RandomFloat() - 0.5f, 0 };
}

inline glm::vec3 DefocusDiskSample(const glm::vec3& center, const glm::vec3& dU, const glm::vec3& dV) {
    glm::vec3 p = RandomInUnitDisk();
    return center + (p[0] * dU) + (p[1] * dV);
}


struct Camera {
    float AspectRatio;
    float VFov = 90.0f;
    glm::vec3 Pixel100Loc;
    glm::vec3 CamCenter;
    glm::vec3 PixelDeltaU;
    glm::vec3 PixelDeltaV;

    glm::vec3 LookFrom = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 LookAt = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 U, V, W;

    float DefocusAngle = 0.6f;
    float FocusDist = 10.0f;
    glm::vec3 DefocusDiskU;
    glm::vec3 DefocusDiskV;

    Camera(const glm::vec3& pos, const glm::vec3& lookat, const glm::vec3& up, float fov, uint32_t width, uint32_t height) {
        VFov = fov;
        LookFrom = pos;
        Up = up;
        CamCenter = LookFrom;
        LookAt = lookat;
        AspectRatio = (float)width / height;

        // float focalLength = glm::length((LookFrom - LookAt));
        float theta = glm::radians(VFov);
        float h = glm::tan(theta / 2);
        // float viewportHeight = 2 * h * focalLength;
        float viewportHeight = 2 * h * FocusDist;
        float viewportWidth = viewportHeight * AspectRatio;

        W = glm::normalize(LookFrom - LookAt);
        U = glm::normalize(glm::cross(Up, W));

        // This might be wrong, we will see...
        V = glm::cross(W, U);

        // glm::vec3 viewportU = { viewportWidth, 0, 0 };
        // Rendering to an image viewportHeight should be negative. It is positive in this case for opengl textures.
        // glm::vec3 viewportV = { 0, viewportHeight, 0 };

        glm::vec3 viewportU = viewportWidth * U;
        // V should be negative for regular images
        glm::vec3 viewportV = viewportHeight * V;

        PixelDeltaU = viewportU / (float)width;
        PixelDeltaV = viewportV / (float)height;

        // glm::vec3 viewportUpperLeft = CamCenter - glm::vec3(0, 0, focalLength) - viewportU/2.f - viewportV/2.f;
        glm::vec3 viewportUpperLeft = CamCenter - (FocusDist * W) - viewportU/2.0f - viewportV/2.0f;
        Pixel100Loc = viewportUpperLeft + .5f * (PixelDeltaU + PixelDeltaV);

        float defocusRadius = FocusDist * glm::tan(glm::radians(DefocusAngle / 2));
        DefocusDiskU = U * defocusRadius;
        DefocusDiskV = V * defocusRadius;
    }

    Ray GetRay(uint32_t x, uint32_t y) {
        glm::vec3 offset = SampleSquare();
        glm::vec3 pixelSample = Pixel100Loc + ((x + offset.x) * PixelDeltaU) + ((y + offset.y) * PixelDeltaV);
        glm::vec3 rayOrigin = (DefocusAngle <= 0) ? CamCenter  : DefocusDiskSample(CamCenter, DefocusDiskU, DefocusDiskV);
        glm::vec3 rayDir = pixelSample - rayOrigin;
        return { rayOrigin, rayDir };
    }
};
