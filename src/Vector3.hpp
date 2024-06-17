#pragma once

#include <iostream>
#include <cmath>
#include <cassert>

namespace Math
{

struct Vector3
{
    float x, y, z;
    Vector3() : x(0), y(0), z(0) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

    bool operator==(const Vector3& v) const {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator!=(const Vector3& v) const {
        return !(*this == v);
    }

    Vector3 operator-() const {
        return Vector3(-x, -y, -z);
    }

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    Vector3 operator+(const Vector3& v) const {
        return Vector3(x + v.x, y + v.y, z + v.z);
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    Vector3 operator-(const Vector3& v) const {
        return Vector3(x - v.x, y - v.y, z - v.z);
    }

    Vector3& operator*=(float s) {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Vector3 operator*(float s) const {
        return Vector3(x * s, y * s, z * s);
    }

    Vector3& operator/=(float s) {
        assert(s != 0);
        float factor = 1 / s;
        x *= factor;
        y *= factor;
        z *= factor;
        return *this;
    }

    Vector3 operator/(float s) const {
        assert(s != 0);
        float factor = 1 / s;
        return Vector3(x * factor, y * factor, z * factor);
    }

    float Dot(Vector3& v) const {
        return (x*v.x + y*v.y + z*v.z);
    }

    float Length() const {
        return std::sqrtf(LengthSquared());
    }

    float LengthSquared() const {
        return x*x + y*y + z*z;
    }

    Vector3& Normalize() {
        float length = Length();
        assert(length != 0);
        *this /= length;
        return *this;
    }

    Vector3 Cross(const Vector3& v) const {
        return Vector3(y*v.z - v.y*z, x*v.z - v.x*z, x*v.y - v.x*y);
    }

};

inline float Dot(const Vector3& u, const Vector3& v) {
    return (u.x*v.x + u.y*v.y + u.z*v.z);
}

inline Vector3 Normalize(const Vector3& v) {
    return v / v.Length();
}

inline std::ostream& operator<<(std::ostream& s, const Vector3& v) {
    s << v.x << ' ' << v.y << ' ' << v.z;
    return s;
}

inline Vector3 Cross(const Vector3& v, const Vector3& u) {
    return Vector3(v.y*u.z - u.y*v.z, v.x*u.z - u.x*v.z, v.x*u.y - u.x*v.y);
}

inline Vector3 operator*(float s, const Vector3& v) {
    return Vector3(s * v.x, s * v.y, s * v.z);
}

inline bool Vector3NearZero(const Vector3& v) {
    float s = 1e-8;
    return (std::fabs(v.x) < s) && (std::fabs(v.y) < s) && (std::fabs(v.z) < s);
}

} // end namespace Math
