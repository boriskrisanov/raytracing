#pragma once
#include <vector>

#include "SceneObject.hpp"

class Scene
{
public:
    std::vector<const SceneObject*> sceneObjects;
    RayIntersection findClosestIntersection(const Ray& ray) const;
};
