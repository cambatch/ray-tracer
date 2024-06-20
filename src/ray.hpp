#pragma once

#include <glm/glm.hpp>


struct Ray {
    glm::vec3 Origin;
    glm::vec3 Direction;

    Ray() : Origin(0), Direction(0) {}
    Ray(const glm::vec3& o, const glm::vec3& d) : Origin(o), Direction(d) {}
    Ray(const Ray& r) : Origin(r.Origin), Direction(r.Direction) {}

    Ray& operator=(const Ray& r) { Origin = r.Origin; Direction = r.Direction; return *this; }

    glm::vec3 At(float t) const { return Origin + Direction * t; }
};
