#include "Camera.hpp"

#include "math.hpp"

Camera::Camera(int width, int height)
    : position(0, 0, 0), rotation(0, 0, 0), fov(70.0)
{
    // TODO: Improve

    constexpr double FOCAL_LENGTH = 1.0;
    const double VIEWPORT_HEIGHT = 2.0 * FOCAL_LENGTH * std::tan(radians(fov/2));
    const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (static_cast<double>(width) / height);
    cameraCenter = {0, 0, 0};

    const Vector3 VIEWPORT_POSITIVE_X{VIEWPORT_WIDTH, 0, 0};
    const Vector3 VIEWPORT_POSITIVE_Y{0, -VIEWPORT_HEIGHT, 0};

    pixelDeltaX = VIEWPORT_POSITIVE_X / width;
    pixelDeltaY = VIEWPORT_POSITIVE_Y / height;

    const Vector3 VIEWPORT_UPPER_LEFT = cameraCenter - Vector3{0, 0, FOCAL_LENGTH} - VIEWPORT_POSITIVE_X / 2 -
        VIEWPORT_POSITIVE_Y / 2;

    originPixel = VIEWPORT_UPPER_LEFT + 0.5 * (pixelDeltaX + pixelDeltaY);
}

Ray Camera::getRayForPixel(int x, int y) const
{
    const auto pixelCenter = originPixel + x * pixelDeltaX + y * pixelDeltaY;
    auto rayDirection = pixelCenter - cameraCenter;
    rayDirection.rotate(rotation, position);
    return {position, rayDirection};
}
