#pragma once

#include <limits>
#include <numbers>
#include <random>


constexpr double PI = std::numbers::pi;
constexpr double PI_2 = std::numbers::pi / 2;
constexpr double INF = std::numeric_limits<double>::infinity();


inline double RandomDouble() {
    static std::mt19937 gen(0);
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gen);
}

inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble();
}

