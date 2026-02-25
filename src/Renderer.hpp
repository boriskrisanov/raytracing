#pragma once

#include <functional>
#include <mutex>
#include <optional>
#include <queue>
#include <span>
#include <thread>
#include <vector>

#include "Scene.hpp"
#include "UI.hpp"

class Random;
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

struct Pixel
{
    int x;
    int y;

    bool operator==(const Pixel& other) const;
};

struct Tile
{
    int xOffset;
    int yOffset;
    std::vector<Pixel> pixels;
    int samplesRemaining = 0;
    int samplesCompleted = 0;
};


class Renderer
{
public:
    Renderer(int width, int height, Scene& scene, Camera& camera);
    ~Renderer();
    void doTileSample(int bounceLimit, Tile* tile, Random& rng);
    void startRenderAsync(int samples, int bounceLimit);
    void stopRender();
    bool isRenderInProgress() const;
    const pixel_buffer& getOutput();
    size_t getCompletedSampleCount();

    Scene& scene;
    Camera& camera;
    bool shadeNormals;
    bool shadeBoundingBoxes;
    bool shadeFirstIntersectionColor = false;
    std::function<void()> onImageUpdate;
private:
    std::vector<std::thread*> renderThreads;
    pixel_buffer finalPixels;
    pixel_buffer sampleSums;
    int completedSampleCount = 0;
    int width;
    int height;
    std::atomic<bool> shouldStopRender = false;
    std::atomic<bool> renderInProgress = false;
    std::atomic<size_t> completedSamples = 0;

    std::vector<Tile> tiles;
    std::mutex tileMutex;
    Tile* requestTile();
    void commitTileSample(Tile* tile, const std::vector<Color>& sample);
    // std::queue<const Tile*> sampleCommitQueue;

    Color traceRay(Ray ray, int bounceLimit, Random& rng) const;
    void clearOutputBuffers();
};
