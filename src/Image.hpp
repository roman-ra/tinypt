#pragma once

#include <cstdint>
#include <memory>

#include "MathLib.hpp"

namespace tinypt
{

template<typename T>
class Image
{
public:
    Image(uint32_t width, uint32_t height):
        m_Width(width), m_Height(height), m_DataPtr(new T[width * height])
    {
    }

    uint32_t getWidth() const { return m_Width; }
    uint32_t getHeight() const { return m_Height; }

    T getPixel(uint32_t x, uint32_t y)
    {
        return m_DataPtr[y * m_Width + x];
    }

    void setPixel(uint32_t x, uint32_t y, const T& value)
    {
        m_DataPtr[y * m_Width + x] = value;
    }

    void tonemap();

    int writePPM(const char* path) const;

private:
    uint32_t m_Width, m_Height;
    std::unique_ptr<T[]> m_DataPtr;
};

typedef Image<Vector3> ImageRGB;

}