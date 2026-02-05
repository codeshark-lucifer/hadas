#pragma once
#include <cmath>

constexpr float PI = 3.14159265358979323846f;
constexpr float DEG2RAD = PI / 180.0f;

namespace mathf
{
    inline float radians(const float &degrees)
    {
        return degrees * (PI / 180.0f);
    }
}