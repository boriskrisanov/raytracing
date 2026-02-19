#include "Camera.hpp"

#include "math.hpp"

// https://raytracing.github.io/books/RayTracingInOneWeekend.html#positionablecamera

Camera::Camera(int width, int height)
    : position(0, 0, 0), rotation(0, 0, 0), fov(70.0)
{
    const Vector3 lookFrom(0, 0, 0);
    const Vector3 lookAt(0, 0, -1);
    const Vector3 viewUp(0, 1, 0);
    cameraCenter = lookFrom;

    Vector3 w = (lookFrom - lookAt).normalised();
    Vector3 u = cross(viewUp, w).normalised();
    Vector3 v = cross(w, u);

    const double FOCAL_LENGTH = (lookFrom - lookAt).magnitude();
    const double VIEWPORT_HEIGHT = 2.0 * FOCAL_LENGTH * std::tan(radians(fov / 2));
    const double VIEWPORT_WIDTH = VIEWPORT_HEIGHT * (static_cast<double>(width) / height);


    Vector3 viewport_u = VIEWPORT_WIDTH * u;    // Vector across viewport horizontal edge
    Vector3 viewport_v = VIEWPORT_HEIGHT * -v;  // Vector down viewport vertical edge


    const Vector3 VIEWPORT_POSITIVE_X{VIEWPORT_WIDTH, 0, 0};
    const Vector3 VIEWPORT_POSITIVE_Y{0, -VIEWPORT_HEIGHT, 0};

    pixelDeltaX = viewport_u / width;
    pixelDeltaY = viewport_v / height;

    const Vector3 VIEWPORT_UPPER_LEFT = cameraCenter - Vector3{0, 0, FOCAL_LENGTH} - viewport_u / 2 -
        viewport_v / 2;

    originPixel = VIEWPORT_UPPER_LEFT + 0.5 * (pixelDeltaX + pixelDeltaY);
}

Ray Camera::getRayForPixel(int x, int y) const
{
    const auto pixelCenter = originPixel + x * pixelDeltaX + y * pixelDeltaY;
    const auto rayDirection = pixelCenter - cameraCenter;
    return {cameraCenter, rayDirection};
}
