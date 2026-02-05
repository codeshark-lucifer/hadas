#pragma once
#include <cmath>

struct vec2
{
    float x, y;

    vec2() : x(0), y(0) {}
    vec2(float v) : x(v), y(v) {}
    vec2(float x, float y) : x(x), y(y) {}

    vec2 operator+(const vec2 &v) const { return {x + v.x, y + v.y}; }
    vec2 operator-(const vec2 &v) const { return {x - v.x, y - v.y}; }
    vec2 operator*(float s) const { return {x * s, y * s}; }
    vec2 operator/(float s) const { return {x / s, y / s}; }

    vec2 &operator+=(const vec2 &v)
    {
        x += v.x;
        y += v.y;
        return *this;
    }

    float Length() const
    {
        return std::sqrt(x * x + y * y);
    }

    vec2 Normalized() const
    {
        float len = Length();
        if (len == 0.0f)
            return vec2(0);
        return *this / len;
    }

    static float Dot(const vec2 &a, const vec2 &b)
    {
        return a.x * b.x + a.y * b.y;
    }
};

struct vec3
{
    float x, y, z;

    vec3() : x(0), y(0), z(0) {}
    vec3(float v) : x(v), y(v), z(v) {}
    vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    vec3 operator+(const vec3 &v) const { return {x + v.x, y + v.y, z + v.z}; }
    vec3 operator-(const vec3 &v) const { return {x - v.x, y - v.y, z - v.z}; }
    vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    vec3 operator/(float s) const { return {x / s, y / s, z / s}; }
    vec3 operator-() const
    {
        return vec3(-x, -y, -z);
    }

    vec3 &operator+=(const vec3 &v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    float Length() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    vec3 Normalized() const
    {
        float len = Length();
        if (len == 0.0f)
            return vec3(0);
        return *this / len;
    }

    static float Dot(const vec3 &a, const vec3 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    static vec3 Cross(const vec3 &a, const vec3 &b)
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
    }
};

struct vec4
{
    float x, y, z, w;

    vec4() : x(0), y(0), z(0), w(0) {}
    vec4(float v) : x(v), y(v), z(v), w(v) {}
    vec4(float x, float y, float z, float w)
        : x(x), y(y), z(z), w(w) {}

    vec4(const vec3 &v, float w)
        : x(v.x), y(v.y), z(v.z), w(w) {}

    vec4 operator+(const vec4 &v) const
    {
        return {x + v.x, y + v.y, z + v.z, w + v.w};
    }

    vec4 operator-(const vec4 &v) const
    {
        return {x - v.x, y - v.y, z - v.z, w - v.w};
    }

    vec4 operator*(float s) const
    {
        return {x * s, y * s, z * s, w * s};
    }

    vec4 operator/(float s) const
    {
        return {x / s, y / s, z / s, w / s};
    }

    vec3 xyz() { return vec3(x, y, z); }
    static float Dot(const vec4 &a, const vec4 &b)
    {
        return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
    }
};

inline vec3 operator*(float s, const vec3 &v)
{
    return {v.x * s, v.y * s, v.z * s};
}