#pragma once

#include <vector>

#include "material.hpp"
#include "sphere.hpp"


struct World {
    std::vector<Sphere> Spheres;
    std::vector<Material> Materials;

    int32_t AddLambertianMaterial(const Colorf& c) {
        size_t idx = Materials.size();
        Materials.emplace_back(c, 0.0f, 0.0f, MaterialType::LAMBERTIAN);
        return idx;
    }

    int32_t AddMetalMaterial(const Colorf& c, float fuzz) {
        size_t idx = Materials.size();
        Materials.emplace_back(c, fuzz, 0.0f, MaterialType::METAL);
        return idx;
    }

    int32_t AddDielectricMaterial(float refractIndex) {
        size_t idx = Materials.size();
        Materials.emplace_back(Colorf{0, 0, 0}, 0.0f, refractIndex, MaterialType::DIELECTRIC);
        return idx;
    }

    void AddSphere(const glm::vec3& o, float r, int32_t matIndex) {
        Spheres.emplace_back(o, r, matIndex);
    }
};
