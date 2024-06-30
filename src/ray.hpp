#pragma once

#include <glm/glm.hpp>


struct Ray {
    glm::vec3 Origin;
    glm::vec3 Direction;
    float     Time;

    Ray() : Origin(0), Direction(0), Time(0) {}
    Ray(const glm::vec3& o, const glm::vec3& d, float t) : Origin(o), Direction(d), Time(t) {}
    Ray(const Ray& r) : Origin(r.Origin), Direction(r.Direction), Time(r.Time) {}

    Ray& operator=(const Ray& r) { Origin = r.Origin; Direction = r.Direction; return *this; }

    glm::vec3 At(float t) const { return Origin + Direction * t; }
};
