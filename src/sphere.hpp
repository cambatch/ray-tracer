#pragma once

#include <vector>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "ray.hpp"
#include "hitRecord.hpp"



struct Sphere {
    glm::vec3 Center;
    float Radius;
    int32_t MatIndex;

    Sphere(const glm::vec3& c, float r, int32_t matIndex)
        : Center(c), Radius(r), MatIndex(matIndex)
    {}
};


inline int32_t HitSphere(const Ray& ray, float tMin, float tMax, HitRecord& rec, const std::vector<Sphere>& spheres) {

    int32_t sphereIdx = -1;
    float closest = tMax;

    for (int32_t i = 0; i < spheres.size(); ++i) {
        const auto& s = spheres[i];

        auto oc = s.Center - ray.Origin;
        auto a = glm::length2(ray.Direction);
        auto b = glm::dot(ray.Direction, oc);
        auto c = glm::length2(oc) - s.Radius*s.Radius;

        float discriminant = b*b - a*c;
        if (discriminant < 0) continue;

        float sqrtd = std::sqrt(discriminant);

        auto root = (b - sqrtd) / a;
        if (root <= tMin || root >= closest) {
            root = (b + sqrtd) / a;
            if (root <= tMin || root >= closest) {
                continue;
            }
        }

        sphereIdx = i;
        closest = root;

        rec.T = root;
        rec.Point = ray.At(root);
        rec.SetFaceNormal(ray, (rec.Point - s.Center) / s.Radius);
        rec.MatIndex = s.MatIndex;
    }

    return sphereIdx;
}
