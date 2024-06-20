#pragma once

#include "utils.hpp"
#include "color.hpp"
#include "ray.hpp"
#include "hitRecord.hpp"


enum class MaterialType {
    LAMBERTIAN = 0,
    METAL,
    DIELECTRIC
};

static float Reflectance(float cosine, float refractionIndex) {
    float r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0*r0;
    return r0 + (1 - r0) * glm::pow((1 - cosine), 5);
}

struct Material {
    Colorf Albedo;
    float fuzz;
    float refractionIndex;
    MaterialType Type;

    void LambertianScatter(const Ray& ray, const HitRecord& record, Colorf& attenuation, Ray& scattered) const {
        glm::vec3 scatterDir = record.Normal + RandomUnitVec3();
        if (NearZero(scatterDir)) {
            scatterDir = record.Normal;
        }

        scattered = Ray(record.Point, scatterDir);
        attenuation = Albedo;
    }

    void MetalScatter(const Ray& ray, const HitRecord& record, Colorf& attenuation, Ray& scattered) const {
        glm::vec3 reflected = glm::reflect(ray.Direction, record.Normal);
        reflected = glm::normalize(reflected) + (fuzz * RandomUnitVec3());
        scattered = Ray(record.Point, reflected);
        attenuation = Albedo;
    }

    void DielectricScatter(const Ray& ray, const HitRecord& record, Colorf& attenuation, Ray& scattered) const {
        attenuation = Colorf(1.0f, 1.0f, 1.0f);
        float ri = record.FrontFace ? (1.0f / refractionIndex) : refractionIndex;

        glm::vec3 unitDir = glm::normalize(ray.Direction);

        float cosTheta = glm::min(glm::dot(-unitDir, record.Normal), 1.0f);
        float sinTheta = glm::sqrt(1.0f - cosTheta*cosTheta);

        bool cannotRefract = ri * sinTheta > 1.0f;
        glm::vec3 dir;

        if (cannotRefract || Reflectance(cosTheta, ri) > RandomFloat()) {
            dir = glm::reflect(unitDir, record.Normal);
        } else {
            dir = glm::refract(unitDir, record.Normal, ri);
        }

        scattered = Ray(record.Point, dir);
    }
};
