#pragma once

#include <glm/glm.hpp>

#include "ray.hpp"


struct HitRecord {
    glm::vec3 Point;
    glm::vec3 Normal;
    float T;
    bool FrontFace;
    int32_t MatIndex = -1;

    void SetFaceNormal(const Ray& r, const glm::vec3& outwardNormal) {
        FrontFace = glm::dot(r.Direction, outwardNormal) < 0;
        Normal = FrontFace ? outwardNormal : -outwardNormal;
    }
};
