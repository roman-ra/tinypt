#include "Scene.hpp"

#include <cstdint>
#include <limits>
#include <cmath>

namespace tinypt
{

struct Intersection {
    uint32_t objectIdx;
    float t;
    Vector3 position;
    Vector3 normal;
};

bool raySphereTest(const Ray &ray, const Sphere &sphere, float &t_out)
{
    Vector3 oc = ray.m_Origin - sphere.m_Center;
    float a = ray.m_Direction.dot(ray.m_Direction);
    float b = 2 * oc.dot(ray.m_Direction);
    float c = oc.dot(oc) - sphere.m_Radius * sphere.m_Radius;
    float discriminant = b * b - 4 * a * c;
    if(discriminant < 0){
        return false;
    }
    else {
        float root = (-b - sqrt(discriminant)) / (2 * a);
        if (root < 0) {
            return false;
        }
        t_out = root;
        return true;
    }
}

bool findIntersection(const Scene &scene, const Ray &ray, 
                      Intersection &intersection)
{
    float t_min = std::numeric_limits<float>::infinity();
    bool hit = false;

    for (uint32_t i = 0; i < scene.m_Objects.size(); i++) {
        float t;
        if (raySphereTest(ray, scene.m_Objects[i], t) && t < t_min) {
            hit = true;
            intersection.t = t;
            intersection.objectIdx = i;
            t_min = t;
        }
    }

    if (hit) {
        intersection.position = ray.m_Origin 
                                + ray.m_Direction * intersection.t;
        intersection.normal = (intersection.position 
                              - scene.m_Objects[intersection.objectIdx].m_Center)
                              .normalized();
    }

    return hit;
}

Vector3 Scene::tracePath(const Ray &ray, uint32_t depth, uint32_t maxDepth)
{
    if (depth > maxDepth) return Vector3{0,0,0};

    Intersection intersection;
    if (findIntersection(*this, ray, intersection)) {
        const Sphere &sphere = m_Objects[intersection.objectIdx];
        const float EPSILON = 1e-6;
        Ray newRay;
        newRay.m_Origin = intersection.position + intersection.normal * EPSILON;
        float cosPhi = 1;

        switch (sphere.m_Material)
        {
        case MaterialType::DIFFUSE:
            newRay.m_Direction = Vector3::random_hemi(intersection.normal);
            cosPhi = newRay.m_Direction.dot(intersection.normal);
            //                    L_i                          Lambertian BRDF         cosPhi
            return tracePath(newRay, depth + 1, maxDepth) * (sphere.m_Color / M_PI) * (cosPhi) * 2 * M_PI;
        case MaterialType::EMISSION:
            return sphere.m_Color;
        case MaterialType::MIRROR:
            newRay.m_Direction = Vector3::reflect(ray.m_Direction, 
                                                intersection.normal);
            return tracePath(newRay, depth + 1, maxDepth) * sphere.m_Color;
        default: return Vector3{0,0,0};
        }
    } else {
        return m_BackgroundColor;
    }
}

}