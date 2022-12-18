#pragma once

#include "MathLib.hpp"

#include <vector>
#include <cstdint>

namespace tinypt
{

enum MaterialType
{
    DIFFUSE, MIRROR, GLASS, EMISSION
};

struct Sphere
{
    Sphere(Vector3 center, float radius, MaterialType material, Vector3 color):
        m_Center(center), m_Radius(radius), m_Material(material), m_Color(color)
    {
    }

    Vector3 m_Center;
    float m_Radius;
    MaterialType m_Material;
    Vector3 m_Color;
};

struct Ray
{
    Vector3 m_Origin;
    Vector3 m_Direction;
};

struct Camera
{
    Vector3 m_Position{0, 0, 0};
    Vector3 m_Right{1,0,0};
    Vector3 m_Up{0,1,0};
    Vector3 m_Forward{0,0,1};
    float m_SensorWidth{0.035f};
    float m_AspectRatio{1.0f};
    float m_FocalLength{0.05f};

    // x and y are between -0.5 and 0.5
    Ray getCameraRay(float x, float y)
    {
        Ray ray;
        ray.m_Origin = m_Position;
        ray.m_Direction = (m_Forward * m_FocalLength
                          + m_Right * m_SensorWidth * x
                          + m_Up * m_SensorWidth * y / m_AspectRatio).normalized();
        return ray;
    }
};

struct Scene
{
    Camera m_Camera;
    std::vector<Sphere> m_Objects;
    Vector3 m_BackgroundColor;

    Vector3 tracePath(const Ray &ray, uint32_t depth, uint32_t maxDepth);
};

}