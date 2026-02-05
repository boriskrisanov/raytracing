#pragma once

#include "math.hpp"
#include "Ray.hpp"

class Camera
{
public:
    Camera(int width, int height);
    Ray getRayForPixel(int x, int y) const;
private:
    Vector3 originPixel;
    Vector3 pixelDeltaX;
    Vector3 pixelDeltaY;
    Vector3 cameraCenter;
};
