#pragma once
#include <vector>

#include "SceneObject.hpp"

class BVH;

class Scene
{
public:
    explicit Scene(std::vector<const SceneObject*> sceneObjects);
    BVH* bvh;
    void computeBvh();
    RayIntersection findClosestIntersection(const Ray& ray) const;
    std::vector<const SceneObject*> sceneObjects;
};
