#pragma once
#include <cstring>
#include <cmath>
#include "vector.h"
#include "quat.h"

struct mat4; // forward declaration
struct mat3
{
    // Column-major (OpenGL style)
    float m[9];

    mat3()
    {
        std::memset(m, 0, sizeof(m));
    }

    static mat3 Identity()
    {
        mat3 r;
        r.m[0] = 1.0f;
        r.m[4] = 1.0f;
        r.m[8] = 1.0f;
        return r;
    }

    static mat3 Translate(const vec2 &t)
    {
        mat3 r = Identity();
        r.m[6] = t.x;
        r.m[7] = t.y;
        return r;
    }

    static mat3 Scale(const vec2 &s)
    {
        mat3 r = Identity();
        r.m[0] = s.x;
        r.m[4] = s.y;
        return r;
    }

    static mat3 Rotate(float radians)
    {
        mat3 r = Identity();
        float c = std::cos(radians);
        float s = std::sin(radians);

        r.m[0] = c;
        r.m[1] = s;
        r.m[3] = -s;
        r.m[4] = c;

        return r;
    }

    vec2 MultiplyPoint(const vec2 &v) const
    {
        return {
            m[0] * v.x + m[3] * v.y + m[6],
            m[1] * v.x + m[4] * v.y + m[7]};
    }

    mat3 operator*(const mat3 &o) const
    {
        mat3 r;
        for (int c = 0; c < 3; c++)
        {
            for (int r0 = 0; r0 < 3; r0++)
            {
                r.m[c * 3 + r0] =
                    m[0 * 3 + r0] * o.m[c * 3 + 0] +
                    m[1 * 3 + r0] * o.m[c * 3 + 1] +
                    m[2 * 3 + r0] * o.m[c * 3 + 2];
            }
        }
        return r;
    }
    mat3(const mat4 &mat4_);

};

struct mat4
{
    float m[16];

    mat4()
    {
        std::memset(m, 0, sizeof(m));
    }

    explicit mat4(float v)
    {
        for (int i = 0; i < 16; i++)
            m[i] = v;
    }

    // Construct mat4 from mat3 (upper-left 3x3), rest identity
    mat4(const mat3 &mat3_)
    {
        m[0] = mat3_.m[0];
        m[1] = mat3_.m[1];
        m[2] = mat3_.m[2];
        m[3] = 0.0f;
        m[4] = mat3_.m[3];
        m[5] = mat3_.m[4];
        m[6] = mat3_.m[5];
        m[7] = 0.0f;
        m[8] = mat3_.m[6];
        m[9] = mat3_.m[7];
        m[10] = mat3_.m[8];
        m[11] = 0.0f;
        m[12] = 0.0f;
        m[13] = 0.0f;
        m[14] = 0.0f;
        m[15] = 1.0f;
    }

    // Construct mat4 from mat3 + translation vec3
    mat4(const mat3 &mat3_, const vec3 &t)
    {
        m[0] = mat3_.m[0];
        m[1] = mat3_.m[1];
        m[2] = mat3_.m[2];
        m[3] = 0.0f;
        m[4] = mat3_.m[3];
        m[5] = mat3_.m[4];
        m[6] = mat3_.m[5];
        m[7] = 0.0f;
        m[8] = mat3_.m[6];
        m[9] = mat3_.m[7];
        m[10] = mat3_.m[8];
        m[11] = 0.0f;
        m[12] = t.x;
        m[13] = t.y;
        m[14] = t.z;
        m[15] = 1.0f;
    }

    // Copy constructor
    mat4(const mat4 &other)
    {
        for (int i = 0; i < 16; i++)
            m[i] = other.m[i];
    }
    static mat4 Identity()
    {
        mat4 r;
        r.m[0] = 1.0f;
        r.m[5] = 1.0f;
        r.m[10] = 1.0f;
        r.m[15] = 1.0f;
        return r;
    }

    static mat4 Translate(const vec3 &t)
    {
        mat4 r = Identity();
        r.m[12] = t.x;
        r.m[13] = t.y;
        r.m[14] = t.z;
        return r;
    }

    static mat4 Scale(const vec3 &s)
    {
        mat4 r = Identity();
        r.m[0] = s.x;
        r.m[5] = s.y;
        r.m[10] = s.z;
        return r;
    }

    static mat4 Rotate(const quat &q)
    {
        quat n = q.Normalized();
        mat4 r = Identity();

        float xx = n.x * n.x;
        float yy = n.y * n.y;
        float zz = n.z * n.z;
        float xy = n.x * n.y;
        float xz = n.x * n.z;
        float yz = n.y * n.z;
        float wx = n.w * n.x;
        float wy = n.w * n.y;
        float wz = n.w * n.z;

        r.m[0] = 1 - 2 * (yy + zz);
        r.m[1] = 2 * (xy + wz);
        r.m[2] = 2 * (xz - wy);

        r.m[4] = 2 * (xy - wz);
        r.m[5] = 1 - 2 * (xx + zz);
        r.m[6] = 2 * (yz + wx);

        r.m[8] = 2 * (xz + wy);
        r.m[9] = 2 * (yz - wx);
        r.m[10] = 1 - 2 * (xx + yy);

        return r;
    }

    mat4 operator*(const mat4 &o) const
    {
        mat4 r;
        for (int c = 0; c < 4; c++)
        {
            for (int r0 = 0; r0 < 4; r0++)
            {
                r.m[c * 4 + r0] =
                    m[0 * 4 + r0] * o.m[c * 4 + 0] +
                    m[1 * 4 + r0] * o.m[c * 4 + 1] +
                    m[2 * 4 + r0] * o.m[c * 4 + 2] +
                    m[3 * 4 + r0] * o.m[c * 4 + 3];
            }
        }
        return r;
    }

    static mat4 Perspective(float fovRadians, float aspect, float near, float far)
    {
        mat4 r;

        float tanHalfFov = std::tan(fovRadians * 0.5f);

        r.m[0] = 1.0f / (aspect * tanHalfFov);
        r.m[5] = 1.0f / tanHalfFov;
        r.m[10] = -(far + near) / (far - near);
        r.m[11] = -1.0f;
        r.m[14] = -(2.0f * far * near) / (far - near);

        return r;
    }

    static mat4 Ortho(float left, float right, float bottom, float top, float near, float far)
    {
        mat4 r = mat4::Identity();

        r.m[0] = 2.0f / (right - left);
        r.m[5] = 2.0f / (top - bottom);
        r.m[10] = -2.0f / (far - near);

        r.m[12] = -(right + left) / (right - left);
        r.m[13] = -(top + bottom) / (top - bottom);
        r.m[14] = -(far + near) / (far - near);

        return r;
    }

    static mat4 LookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
    {
        vec3 f = (center - eye).Normalized();     // forward
        vec3 s = vec3::Cross(f, up).Normalized(); // right
        vec3 u = vec3::Cross(s, f);               // up

        mat4 r = mat4::Identity();

        r.m[0] = s.x;
        r.m[1] = u.x;
        r.m[2] = -f.x;

        r.m[4] = s.y;
        r.m[5] = u.y;
        r.m[6] = -f.y;

        r.m[8] = s.z;
        r.m[9] = u.z;
        r.m[10] = -f.z;

        r.m[12] = -vec3::Dot(s, eye);
        r.m[13] = -vec3::Dot(u, eye);
        r.m[14] = vec3::Dot(f, eye);

        return r;
    }
};


