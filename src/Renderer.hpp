#pragma once

#include <functional>
#include <thread>
#include <vector>

#include "Scene.hpp"
#include "UI.hpp"

class Camera;
struct Ray;
struct Vector3;

using Color = Vector3;
using pixel_buffer = std::vector<std::vector<Color>>;

// TODO: Dynamic window resizing

// class RenderWorker
// {
// public:
//     void start(int width, int height, Scene& scene, Camera& camera, int samples, int bounceLimit);
//     void stop();
// private:
//     bool shouldStop = false;
// };

class Renderer
{
public:
    Renderer(int width, int height, Scene& scene, Camera& camera);
    ~Renderer();
    void render(int samples, int bounceLimit);
    void startRenderAsync(int samples, int bounceLimit);
    void stopRender();
    bool isRenderInProgress() const;
    const pixel_buffer& getOutput();
    size_t getCompletedSampleCount();

    Scene& scene;
    Camera& camera;
    bool shadeNormals;
    bool shadeBoundingBoxes;
    bool shadeFirstIntersectionColor;
    std::function<void()> onImageUpdate;
private:
    // Needs to be on the heap otherwise causes access violations when reassigning (only sometimes)
    // (thread destructor tries to delete &this capture?)
    // https://stackoverflow.com/questions/25559918/c-stdthread-crashes-upon-execution
    std::thread renderThread;
    pixel_buffer finalPixels;
    pixel_buffer sampleSums;
    int completedSampleCount = 0;
    int width;
    int height;
    std::atomic<bool> shouldStopRender = false;
    std::atomic<bool> renderInProgress = false;
    std::atomic<size_t> completedSamples = 0;

    Color traceRay(Ray ray, int bounceLimit) const;
    void clearOutputBuffers();
};
