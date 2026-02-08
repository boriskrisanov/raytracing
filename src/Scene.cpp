#include "Scene.hpp"

#include "Ray.hpp"
#include "SceneObject.hpp"

RayIntersection Scene::findClosestIntersection(const Ray& ray) const
{
    RayIntersection closestHit{};
    for (const SceneObject* sceneObject : sceneObjects)
    {
        RayIntersection hit = sceneObject->intersects(ray, {0.0001, 100000000});
        if (hit.didHit)
        {
            hit.material = sceneObject->material;
            if (!closestHit.didHit || hit.rayParameter < closestHit.rayParameter)
            {
                closestHit = hit;
            }
        }
    }
    return closestHit;
}
