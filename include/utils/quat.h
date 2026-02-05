#pragma once
#include <cmath>
#include "vector.h"

#define M_PI 3.14159265f

struct quat
{
    float w, x, y, z;

    quat() : w(1), x(0), y(0), z(0) {}
    quat(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

    static quat Identity()
    {
        return quat();
    }

    static quat FromAxisAngle(const vec3 &axis, float radians)
    {
        float half = radians * 0.5f;
        float s = std::sin(half);
        return {
            std::cos(half),
            axis.x * s,
            axis.y * s,
            axis.z * s};
    }

    quat Normalized() const
    {
        float len = std::sqrt(w * w + x * x + y * y + z * z);
        return {w / len, x / len, y / len, z / len};
    }

    quat operator*(const quat &q) const
    {
        return {
            w * q.w - x * q.x - y * q.y - z * q.z,
            w * q.x + x * q.w + y * q.z - z * q.y,
            w * q.y - x * q.z + y * q.w + z * q.x,
            w * q.z + x * q.y - y * q.x + z * q.w};
    }
    static quat FromEuler(const vec3 &eulerDegrees)
    {
        // Convert degrees → radians
        vec3 r = {
            eulerDegrees.x * (float)M_PI / 180.0f,
            eulerDegrees.y * (float)M_PI / 180.0f,
            eulerDegrees.z * (float)M_PI / 180.0f};

        float cx = std::cos(r.x * 0.5f);
        float sx = std::sin(r.x * 0.5f);
        float cy = std::cos(r.y * 0.5f);
        float sy = std::sin(r.y * 0.5f);
        float cz = std::cos(r.z * 0.5f);
        float sz = std::sin(r.z * 0.5f);

        // Pitch (X), Yaw (Y), Roll (Z)
        quat q;
        q.w = cx * cy * cz + sx * sy * sz;
        q.x = sx * cy * cz - cx * sy * sz;
        q.y = cx * sy * cz + sx * cy * sz;
        q.z = cx * cy * sz - sx * sy * cz;

        return q.Normalized();
    }
};

inline vec3 operator*(const quat &q, const vec3 &v)
{
    vec3 u{q.x, q.y, q.z};
    float s = q.w;

    return u * (2.0f * vec3::Dot(u, v)) +
           v * (s * s - vec3::Dot(u, u)) +
           vec3::Cross(u, v) * (2.0f * s);
}
