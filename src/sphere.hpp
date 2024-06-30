#pragma once

#include <vector>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include "ray.hpp"
#include "hitRecord.hpp"



struct Sphere {
    glm::vec3 CenterS;
    glm::vec3 CenterE;
    float     Radius;
    int32_t   MatIndex;
    bool      IsMoving;

    Sphere(const glm::vec3& c, float r, int32_t matIndex)
        : CenterS(c), Radius(r), MatIndex(matIndex), IsMoving(false)
    {}

    Sphere(const glm::vec3& c1, const glm::vec3& c2, float r, int32_t matIndex, bool moving)
        : CenterS(c1), CenterE(c2 - c1), Radius(r), MatIndex(matIndex), IsMoving(moving)
    {}

    glm::vec3 SphereCenter(float t) const {
        return CenterS + t*CenterE;
    }
};


inline int32_t HitSphere(const Ray& ray, float tMin, float tMax, HitRecord& rec, const std::vector<Sphere>& spheres) {

    int32_t sphereIdx = -1;
    float closest = tMax;

    for (int32_t i = 0; i < spheres.size(); ++i) {
        const auto& s = spheres[i];

        glm::vec3 center = s.IsMoving ? s.SphereCenter(ray.Time) : s.CenterS;
        // auto oc = s.CenterS - ray.Origin;
        auto oc =  center - ray.Origin;
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
        rec.SetFaceNormal(ray, (rec.Point - s.CenterS) / s.Radius);
        rec.MatIndex = s.MatIndex;
    }

    return sphereIdx;
}
