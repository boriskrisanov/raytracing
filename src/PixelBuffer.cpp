#include "PixelBuffer.hpp"

// TODO: Replace 2D arrays with this

PixelBuffer::PixelBuffer(int width, int height)
    : size(width * height)
{
    for (int i = 0; i < width * height; i++)
    {
        buffer.push_back({});
    }
    // Size is a known constant, so over-allocating wastes memory
    buffer.shrink_to_fit();
}

// int PixelBuffer::computeIndex(int x, int y)
// {
//     return (x % )
// }
