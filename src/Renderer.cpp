#include "Renderer.hpp"

#include "Camera.hpp"
#include "Material.hpp"
#include "math.hpp"
#include "Ray.hpp"
#include "SceneObject.hpp"

Renderer::Renderer(int width, int height, Scene& scene, Camera& camera)
    : scene(scene), camera(camera), width(width), height(height)
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
    clearOutputBuffers();
    for (sampleCount = 1; sampleCount <= samples; sampleCount++)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                const Ray ray = camera.getRayForPixel(i, j);
                sampleSums[i][j] += traceRay(ray, bounceLimit);
                finalPixels[i][j] = sampleSums[i][j] / sampleCount;
            }
        }
    }
}

void Renderer::startRenderAsync(int samples, int bounceLimit)
{
    clearOutputBuffers();
    renderThread = std::thread([this, samples, bounceLimit]
    {
        //  Probably not the best design when it comes to having multiple render threads later
        // TODO: Refactor
        this->render(samples, bounceLimit);
    });
}

const pixel_buffer& Renderer::getOutput()
{
    return finalPixels;
}

Color Renderer::traceRay(Ray ray, int bounceLimit) const
{
    Color rayColor = {1, 1, 1};
    Color incomingLight = {0, 0, 0};

    for (int i = 0; i < bounceLimit; i++)
    {
        RayIntersection h = scene.findClosestIntersection(ray);

        if (!h.didHit)
        {
            // Currently always returns 0 because hitting a light will cause the ray to be absorbed , so this will never
            //  be reached if incomingLight is non-zero
            // break;
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
        rayColor *= scatteredRay.value().color;
        ray = scatteredRay.value().ray;
    }

    return rayColor * incomingLight;
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
