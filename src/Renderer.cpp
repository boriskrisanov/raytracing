#include "Renderer.hpp"

#include <chrono>
#include <iostream>

#include "Camera.hpp"
#include "Material.hpp"
#include "math.hpp"
#include "MeshInstance.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

bool Pixel::operator==(const Pixel& other) const
{
    return x == other.x && y == other.y;
}

Renderer::Renderer(int width, int height, Scene& scene, Camera& camera)
    : scene(scene), camera(camera), shadeNormals(false), shadeBoundingBoxes(false), width(width), height(height)
{
    for (int i = 0; i < width; i++)
    {
        finalPixels.push_back(std::vector<Color>{});
        sampleSums.push_back(std::vector<Color>{});
        for (int j = 0; j < height; j++)
        {
            finalPixels[i].push_back(Color{0, 0, 0});
            sampleSums[i].push_back(Color{0, 0, 0});
        }
    }
    // std::vector will grow and allocate space for more pixels than we have, so this avoids wasting memory since we
    // know there won't be any more pixels
    finalPixels.shrink_to_fit();
    sampleSums.shrink_to_fit();
    tiles.clear();

    // Generate tiles
    constexpr int tileSize = 64;
    std::vector<Pixel> currentTilePixels;
    int xOffset = 0;
    int yOffset = 0;
    while (xOffset < width)
    {
        while (yOffset < height)
        {
            for (int tileX = 0; tileX < tileSize; tileX++)
            {
                if (xOffset + tileX >= width)
                {
                    break;
                }
                for (int tileY = 0; tileY < tileSize; tileY++)
                {
                    if (yOffset + tileY >= height)
                    {
                        break;
                    }
                    currentTilePixels.push_back({xOffset + tileX, yOffset + tileY});
                }
            }
            yOffset += tileSize;
            tiles.push_back(Tile{xOffset, yOffset, currentTilePixels, 0});
            currentTilePixels.clear();
        }
        xOffset += tileSize;
        yOffset = 0;
    }
}

Renderer::~Renderer()
{
    // stopRender();
}

void Renderer::doTileSample(int bounceLimit, Tile* tile)
{
    renderInProgress = true;
    std::vector<Color> samples;

    for (const auto [i, j] : tile->pixels)
    {
        // TODO: UI option to show tiles
        // int tileN = 0;
        // for (Tile tile : tiles)
        // {
        //     if (std::ranges::find(tile.pixels, Pixel{i, j}) != tile.pixels.end())
        //     {
        //         break;
        //     }
        //     tileN++;
        //  }
        // finalPixels[i][j] = tileN % 2 == 0 ? Color{1, 0, 0} : Color{0, 0, 1};
        // continue;
        if (shouldStopRender) [[unlikely]]
        {
            break;
        }
        // TODO: Cache rays
        const Ray ray = camera.getRayForPixel(i, j);
        samples.push_back(traceRay(ray, bounceLimit));
    }

    commitTileSample(tile, samples);
}

void Renderer::startRenderAsync(int samples, int bounceLimit)
{
    if (isRenderInProgress())
    {
        throw std::runtime_error{"startRenderAsync called while render is already running"};
    }

    clearOutputBuffers();
    shouldStopRender = false;
    for (std::thread* thread : renderThreads)
    {
        if (thread->joinable())
        {
            thread->join();
        }
    }
    for (Tile& tile : tiles)
    {
        tile.samplesRemaining = samples;
        tile.samplesCompleted = 0;
    }
    for (int i = 0; i < 10; i++)
    {
        auto thread = new std::thread([this, bounceLimit]
        {
            //  Probably not the best design when it comes to having multiple render threads later
            // TODO: Refactor
            while (Tile* tile = this->requestTile())
            {
                this->doTileSample(bounceLimit, tile);
            }
        });
        renderThreads.push_back(thread);
    }
}

void Renderer::stopRender()
{
    shouldStopRender = true;
    for (std::thread* thread : renderThreads)
    {
        if (thread->joinable())
        {
            thread->join();
        }
    }
}

bool Renderer::isRenderInProgress() const
{
    return !shouldStopRender && renderInProgress;
}

const pixel_buffer& Renderer::getOutput()
{
    return finalPixels;
}

size_t Renderer::getCompletedSampleCount()
{
    return completedSamples;
}

Tile* Renderer::requestTile()
{
    tileMutex.lock();

    Tile* tileWithMostSamplesRemaining = &tiles[0];
    for (Tile& tile : tiles)
    {
        if (tile.samplesRemaining > tileWithMostSamplesRemaining->samplesRemaining)
        {
            tileWithMostSamplesRemaining = &tile;
        }
    }

    tileMutex.unlock();

    if (tileWithMostSamplesRemaining->samplesRemaining == 0)
    {
        return nullptr;
    }

    tileWithMostSamplesRemaining->samplesRemaining--;

    return tileWithMostSamplesRemaining;
}

void Renderer::commitTileSample(Tile* tile, const std::vector<Color>& sample)
{
    tileMutex.lock();

    tile->samplesCompleted++;

    for (int i = 0; i < tile->pixels.size(); i++)
    {
        const auto [x, y] = tile->pixels[i];
        sampleSums[x][y] += sample[i];
        finalPixels[x][y] = sampleSums[x][y] / tile->samplesCompleted;
    }

    tileMutex.unlock();
}

Color Renderer::traceRay(Ray ray, int bounceLimit) const
{
    Color debugOverlayColor = {1, 1, 1};
    Color debugOverlayBackground = {0, 0, 0};

    const RayIntersection h = scene.findClosestIntersection(ray);

    if (shadeFirstIntersectionColor)
    {
        if (h.didHit)
        {
            std::optional<ScatteredRay> scatteredRay = h.material->scatter(ray, h);
            if (scatteredRay.has_value())
            {
                return scatteredRay.value().color;
            }
        }
        return {};
    }


    if (shadeBoundingBoxes)
    {
        for (auto sceneObject : scene.sceneObjects)
        {
            if (auto meshInstance = dynamic_cast<const MeshInstance*>(sceneObject))
            {
                // if (meshInstance->intersectsBoundingBox(ray, {0.0001, 10000000000}))
                // {
                //     debugOverlayColor = {1, 0.5, 0.5};
                //     debugOverlayBackground = {0.5, 0, 0};
                // }
                if (meshInstance->intersectsBvhNode(ray, meshInstance->bvh->getLeft()))
                {
                    debugOverlayColor = {1, 0.5, 0.5};
                    debugOverlayBackground = {0.5, 0, 0};
                }
                if (meshInstance->intersectsBvhNode(ray, meshInstance->bvh->getRight()))
                {
                    debugOverlayColor *= {0.5, 0.5, 1};
                    debugOverlayBackground += {0, 0, 0.5};
                }
                // TODO: Fix
                if (meshInstance->intersectsBoundingBoxNearEdge(ray, {0.0001, 100000000}, 0.1))
                {
                    // return {1, 0, 0};
                }
            }
        }
    }

    if (shadeNormals)
    {
        if (h.didHit)
        {
            return 0.5 * Vector3{
                h.normal.x + 1,
                h.normal.y + 1,
                h.normal.z + 1
            } * debugOverlayColor;
        }
        return debugOverlayBackground;
    }

    Color rayColor = {1, 1, 1};
    Color incomingLight = {0, 0, 0};

    for (int i = 0; i < bounceLimit; i++)
    {
        RayIntersection h = scene.findClosestIntersection(ray);
        if (!h.didHit)
        {
            break;

            // Sky
            auto a = 0.5 * (ray.direction.normalised().y + 1.0);
            const Vector3 skyColor = (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
            return rayColor * skyColor;
        }

        incomingLight += h.material->emit();

        std::optional<ScatteredRay> scatteredRay = h.material->scatter(ray, h);
        if (!scatteredRay.has_value())
        {
            // Ray was absorbed
            break;
        }
        // double x = scatteredRay.value().ray.direction.y < 0 ? 0.5 : 1.0;
        // return Vector3{x, x, x} * 0.5;
        rayColor *= scatteredRay.value().color;
        ray = scatteredRay.value().ray;
    }

    return rayColor * incomingLight * debugOverlayColor;
}

void Renderer::clearOutputBuffers()
{
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            finalPixels[i][j] = {0, 0, 0};
            sampleSums[i][j] = {0, 0, 0};
        }
    }
}
