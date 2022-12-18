#include "Image.hpp"
#include "Scene.hpp"

#include <cstdint>
#include <thread>
#include <iostream>
#include <random>

#define WIDTH       640
#define HEIGHT      640
#define THREADS     16
#define SPP         100
#define FILTER_SIZE 0.5f
#define MAX_DEPTH   2

using namespace tinypt;

struct RenderingContext {
    uint32_t renderWidth;
    uint32_t renderHeight;
    uint32_t spp;
    float filterSize;
    uint32_t maxDepth;
    Scene *scene;
    ImageRGB *output;
};


void renderPixel(uint32_t x, uint32_t y, const RenderingContext *rc)
{
    Vector3 pixel{0,0,0};
    static thread_local std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(-rc->filterSize, rc->filterSize);
    
    for (uint32_t i = 0; i < rc->spp; i++) {
        float filterX = distribution(generator);
        float filterY = distribution(generator);
        float subpixelX = ((float)x + 0.5f + filterX) 
                            / rc->renderWidth - 0.5f; 
        float subpixelY = 0.5f - ((float)y + 0.5f + filterY) 
                            / rc->renderHeight;
        Ray cameraRay = rc->scene->m_Camera.getCameraRay(subpixelX, subpixelY);
        pixel = pixel + rc->scene->tracePath(cameraRay, 0, rc->maxDepth) 
                        * 1 / rc->spp;
    }

    rc->output->setPixel(x, y, pixel);
}

void renderTile(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1, 
                const RenderingContext *rc)
{
    for (uint32_t y = y0; y < y1; y++) {
        for (uint32_t x = x0; x < x1; x++) {
            renderPixel(x, y, rc);
        }
    }
}

void render(uint32_t numThreads, const RenderingContext *rc)
{
    rc->scene->m_Camera.m_AspectRatio = (float)rc->renderWidth/rc->renderHeight;
    
    const uint32_t tileWidth = (rc->output->getWidth() + (numThreads - 1)) / numThreads;
    std::thread threads[numThreads];

    for (uint32_t i = 0; i < numThreads; i++) {
        uint32_t x0 = tileWidth * i;
        uint32_t x1 = tileWidth * (i + 1);
        if (x1 > rc->renderWidth) {
            x1 = rc->renderWidth;
        }
        threads[i] = std::thread(renderTile, x0, 0, x1, rc->renderHeight, rc);
    }

    for (uint32_t i = 0; i < numThreads; i++) {
        threads[i].join();
    }
}

int main(int argc, char *argv[])
{
    ImageRGB output(WIDTH, HEIGHT);

    Scene scene;
    //scene.m_BackgroundColor = Vector3{0.52, 0.8, 0.92} * 3;
    scene.m_BackgroundColor = Vector3{1, 1, 1} * 2;
    scene.m_Camera.m_FocalLength = 0.035f;
    scene.m_Camera.m_Position.y = 1;

    scene.m_Objects.emplace_back(Vector3{0,-10001,0}, 10000.0f, MaterialType::DIFFUSE, Vector3{0.8,0.8,0.8});
    
    scene.m_Objects.emplace_back(Vector3{-1,-0.5,5}, 0.5, MaterialType::EMISSION, Vector3{1,0,0} * 200);
    scene.m_Objects.emplace_back(Vector3{-2,-0.5,8}, 0.5, MaterialType::EMISSION, Vector3{0,1,0} * 200);
    scene.m_Objects.emplace_back(Vector3{2,-0.5,8}, 0.5, MaterialType::EMISSION, Vector3{0,0,1} * 200);

    scene.m_Objects.emplace_back(Vector3{1,-0.5,5}, 0.5, MaterialType::MIRROR, Vector3{0.85,0.85,0.85});
    scene.m_Objects.emplace_back(Vector3{0,0,7}, 1, MaterialType::DIFFUSE, Vector3{1,1,1});

    scene.m_Objects.emplace_back(Vector3{0,-1,15}, 5.0f, MaterialType::DIFFUSE, Vector3{0.2,0.2,0.2});

    RenderingContext rc{WIDTH, HEIGHT, SPP, FILTER_SIZE, MAX_DEPTH, &scene, &output};
    render(THREADS, &rc);

    output.tonemap();
    output.writePPM("out.ppm");

    return 0;
}