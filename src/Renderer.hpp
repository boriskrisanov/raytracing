#pragma once

#include <thread>
#include <vector>

#include "Scene.hpp"

class Camera;
struct Ray;
struct Vector3;

using Color = Vector3;
using pixel_buffer = std::vector<std::vector<Color>>;

// TODO: Dynamic window resizing

class Renderer
{
public:
    Renderer(int width, int height, Scene& scene, Camera& camera);
    void render(int samples, int bounceLimit);
    void startRenderAsync(int samples, int bounceLimit);
    void stopRender();
    const pixel_buffer& getOutput();

    Scene& scene;
    Camera& camera;
private:
    std::thread renderThread;
    pixel_buffer finalPixels;
    pixel_buffer sampleSums;
    int sampleCount = 0;
    int width;
    int height;

    Color traceRay(Ray ray, int bounceLimit) const;
    void clearOutputBuffers();
};
