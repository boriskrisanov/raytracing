#include "Renderer.hpp"

#include <chrono>
#include <iostream>

#include "Camera.hpp"
#include "Material.hpp"
#include "math.hpp"
#include "MeshInstance.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

Renderer::Renderer(int width, int height, Scene& scene, Camera& camera)
    : scene(scene), camera(camera), shadeNormals(false), width(width), height(height)
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
}

void Renderer::render(int samples, int bounceLimit)
{
    auto start = std::chrono::system_clock::now();
    shouldStopRender = false;
    renderInProgress = true;
    clearOutputBuffers();
    for (completedSampleCount = 1; completedSampleCount <= samples; completedSampleCount++)
    {
        for (int i = 0; i < width; i++)
        {
            if (shouldStopRender) [[unlikely]]
            {
                break;
            }
            for (int j = 0; j < height; j++)
            {
                if (shouldStopRender) [[unlikely]]
                {
                    break;
                }
                const Ray ray = camera.getRayForPixel(i, j);
                sampleSums[i][j] += traceRay(ray, bounceLimit);
                finalPixels[i][j] = sampleSums[i][j] / completedSampleCount;
            }
        }
    }
    renderInProgress = false;
    auto end = std::chrono::system_clock::now();
    std::cout << "debug: render time " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start) << std::endl;
}

void Renderer::startRenderAsync(int samples, int bounceLimit)
{
    if (isRenderInProgress())
    {
        throw std::runtime_error{"startRenderAsync called while render is already running"};
    }

    clearOutputBuffers();
    renderThread = std::thread([this, samples, bounceLimit]
    {
        //  Probably not the best design when it comes to having multiple render threads later
        // TODO: Refactor
        this->render(samples, bounceLimit);
    });}

void Renderer::stopRender()
{
    shouldStopRender = true;
    renderThread.join();
}

bool Renderer::isRenderInProgress() const
{
    return !shouldStopRender && renderInProgress;
}

const pixel_buffer& Renderer::getOutput()
{
    return finalPixels;
}

Color Renderer::traceRay(Ray ray, int bounceLimit) const
{
    if (shadeNormals)
    {
        RayIntersection h = scene.findClosestIntersection(ray);

        if (h.didHit)
        {
            return 0.5 * Vector3{
                h.normal.x + 1,
                h.normal.y + 1,
                h.normal.z + 1
            };
        }
        return {};
    }

    // TODO: Visualise bounding boxes
    Color debugOverlayColor = {1, 1, 1};
    RayIntersection h = scene.findClosestIntersection(ray);
    // for (const SceneObject* o : scene.sceneObjects)
    // {
    //     if (const MeshInstance* m = dynamic_cast<const MeshInstance*>(o))
    //     {
    //         if (m->boundingBox.intersectsRay(ray))
    //         {
    //             debugOverlayColor = {1, 0.5, 0.5};
    //         }
    //     }
    // }

    // if (h.hitBoundingBox) return {1, 1, 1};
    // return {};


    Color rayColor = {1, 1, 1};
    Color incomingLight = {0, 0, 0};

    for (int i = 0; i < bounceLimit; i++)
    {
        RayIntersection h = scene.findClosestIntersection(ray);

        if (!h.didHit)
        {
            // Sky
            auto a = 0.5 * (ray.direction.normalised().y + 1.0);
            const Vector3 skyColor = (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
            return rayColor * skyColor * debugOverlayColor;
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
