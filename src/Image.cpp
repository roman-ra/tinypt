#include "Image.hpp"

#include <fstream>
#include <memory>

namespace tinypt {

// http://filmicworlds.com/blog/filmic-tonemapping-operators/
float Uncharted2Tonemap(float x)
{
    const float A = 0.15;
    const float B = 0.50;
    const float C = 0.10;
    const float D = 0.20;
    const float E = 0.02;
    const float F = 0.30;
    const float W = 11.2;
    return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}

template<>
void Image<Vector3>::tonemap()
{
    for (uint32_t i = 0; i < m_Width * m_Height; i++) {
        m_DataPtr[i].r = Uncharted2Tonemap(m_DataPtr[i].r);
        m_DataPtr[i].g = Uncharted2Tonemap(m_DataPtr[i].g);
        m_DataPtr[i].b = Uncharted2Tonemap(m_DataPtr[i].b);
    }
}


template<>
int Image<Vector3>::writePPM(const char* path) const
{
    std::ofstream output;
    output.open(path);
    if (!output.is_open()) {
        return -1;
    }

    std::unique_ptr<unsigned char[]> rgbBytes(new unsigned char[m_Width * m_Height * 3]);

    // No clamping is done
    for (uint32_t i = 0; i < m_Width * m_Height; i++) {
        rgbBytes[3 * i + 0] = static_cast<unsigned char>(m_DataPtr[i].r * 255.0f);
        rgbBytes[3 * i + 1] = static_cast<unsigned char>(m_DataPtr[i].g * 255.0f);
        rgbBytes[3 * i + 2] = static_cast<unsigned char>(m_DataPtr[i].b * 255.0f);
    }

    output << "P6\n" << m_Width << " " << m_Height << "\n255\n";

    output.write(reinterpret_cast<const char*>(rgbBytes.get()), m_Width * m_Height * 3 * sizeof(unsigned char));

    output.close();
    return 0;
}

}
