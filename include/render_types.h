#pragma once
#include "utils/mathf.h"
#include "assets.h"
#include "utils.h"
#include <cstring> // for memset if needed

// constexpr int MAX_TRANSFORMS = 1000;

template <typename T>
class Array
{
public:
    Array() = default;

    ~Array()
    {
        delete[] arr; // free heap memory
        arr = nullptr;
    }

    size_t size() const { return count; }
    size_t capacity() const { return cap; }

    T* data() { return arr; }
    const T* data() const { return arr; }

    T& operator[](size_t i)
    {
        LOG_ASSERT(i < count, "Array index out of bounds");
        return arr[i];
    }

    void push(const T& value)
    {
        if (count >= cap)
            grow();

        arr[count++] = value;
    }

    void resize(size_t newSize)
    {
        if (newSize > cap)
            reserve(newSize);

        // Default-initialize new elements
        for (size_t i = count; i < newSize; ++i)
            arr[i] = T{};

        count = newSize;
    }

    void clear()
    {
        count = 0; // just reset size, keep capacity
    }

private:
    size_t count = 0;
    size_t cap = 0;
    T* arr = nullptr;

    void grow()
    {
        size_t newCap = cap == 0 ? 2 : cap * 2;
        T* newData = new T[newCap];

        for (size_t i = 0; i < count; i++)
            newData[i] = arr[i];

        delete[] arr;
        arr = newData;
        cap = newCap;
    }

    void reserve(size_t newCap)
    {
        if (newCap <= cap)
            return;

        T* newData = new T[newCap];

        for (size_t i = 0; i < count; i++)
            newData[i] = arr[i];

        delete[] arr;
        arr = newData;
        cap = newCap;
    }
};

struct Camera2D
{
    float zoom{1.0f};
    vec2 dimentions{956.0f, 540.0f}; // screen width , hright
    vec2 position{0.0f, 0.0f};
    mat4 matrix()
    {
        mat4 p, v;

        p = mat4::Ortho(
            -dimentions.x * 0.5f * zoom,
            dimentions.x * 0.5f * zoom,
            -dimentions.y * 0.5f * zoom,
            dimentions.y * 0.5f * zoom,
            -1, 1);

        // camera moves opposite of the world
        v = mat4::Translate({-position.x, -position.y, 0.0f});

        return p * v;
    }
};

struct Transform
{
    ivec2 ioffset;
    ivec2 isize;

    vec2 pos;
    vec2 size;
};

struct RenderData
{
    Camera2D gameCamera;
    Camera2D uiCamera;

    Array<Transform> transforms;
};
