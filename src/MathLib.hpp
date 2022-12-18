#pragma once

#include <cmath>
#include <random>

namespace tinypt {

struct Vector3
{
    Vector3(): x(0), y(0), z(0) { }
    Vector3(float x, float y, float z): x(x), y(y), z(z) { }

    union {
        struct { float x, y, z; };
        struct { float r, g, b; };
    };

    Vector3 operator-() const
    {
        return Vector3{-x, -y, -z};
    }

    float sqrlength() const
    {
        return x * x + y *y + z * z;
    }

    float length() const 
    {
        return sqrtf(sqrlength());
    }

    Vector3 normalized() const 
    {
        float l = length();
        return Vector3{x / l, y / l, z / l};
    }

    float dot(const Vector3 &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3 cross(const Vector3 &v) const {
        return Vector3{
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        };
    }

    static Vector3 reflect(const Vector3 &v, const Vector3 &n);
    static Vector3 random_hemi(const Vector3& n);
};

inline Vector3 operator+(const Vector3 &a, const Vector3 &b)
{
    return Vector3{a.x + b.x, a.y + b.y, a.z + b.z};
}

inline Vector3 operator+(const Vector3 &a, float s)
{
    return Vector3{a.x + s, a.y + s, a.z + s};
}

inline Vector3 operator-(const Vector3 &a, const Vector3 &b)
{
    return Vector3{a.x - b.x, a.y - b.y, a.z - b.z};
}

inline Vector3 operator-(const Vector3 &a, float s)
{
    return Vector3{a.x - s, a.y - s, a.z - s};
}

inline Vector3 operator*(const Vector3 &v, float s)
{
    return Vector3{v.x * s, v.y * s, v.z * s};
}

inline Vector3 operator*(const Vector3 &a, const Vector3 &b)
{
    return Vector3{a.x * b.x, a.y * b.y, a.z * b.z};
}

inline Vector3 operator/(const Vector3 &v, float s)
{
    return Vector3{v.x / s, v.y / s, v.z / s};
}

inline Vector3 Vector3::reflect(const Vector3 &v, const Vector3 &n)
{
    return v - (n * (2 * v.dot(n))); 
}

inline Vector3 Vector3::random_hemi(const Vector3& n)
{
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
    
    Vector3 rnd{distribution(generator), 
                distribution(generator), 
                distribution(generator)}; 
    
    if (n.dot(rnd) < 0) {
        rnd = -rnd;
    }

    return rnd;
}

}