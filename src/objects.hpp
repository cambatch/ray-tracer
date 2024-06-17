#pragma once

#include <vector>

#include "utils.hpp"
#include "Ray.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>



struct HitRecord {
    glm::vec3 Point;
    glm::vec3 Normal;
    double T;
    bool FrontFace;

    void SetFaceNormal(const Ray& r, const glm::vec3& outwardNormal) {
        FrontFace = glm::dot(r.Direction, outwardNormal) < 0;
        Normal = FrontFace ? outwardNormal : -outwardNormal;
    }
};


struct Sphere {
    glm::vec3 Center;
    double Radius;

    Sphere(const glm::vec3& c, double r)
        : Center(c), Radius(r)
    {}

    bool Hit(const Ray& r, double rayTMin, double rayTMax, HitRecord& record) const {
        auto oc = Center - r.Origin;
        auto a = glm::length2(r.Direction);
        auto b = glm::dot(r.Direction, oc);
        auto c = glm::length2(oc) - Radius*Radius;

        auto discriminant = b*b - a*c;
        if (discriminant < 0) return false;

        double sqrtd = std::sqrt(discriminant);

        auto root = (b - sqrtd) / a;
        if (root <= rayTMin || root >= rayTMax) {
            root = (b + sqrtd) / a;
            if (root <= rayTMin || root >= rayTMax) {
                return false;
            }
        }

        record.T = root;
        record.Point = r.At(root);
        record.SetFaceNormal(r, (record.Point - Center) / (float)Radius);

        return true;
    }
};


struct World {
    std::vector<Sphere> Spheres;

    void AddSphere(const glm::vec3& o, double r) {
        Spheres.push_back(Sphere(o, r));
    }
};
