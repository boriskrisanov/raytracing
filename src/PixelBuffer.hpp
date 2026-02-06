#pragma once
#include <vector>

#include "Renderer.hpp"

// TODO: Replace 2D arrays with this

class PixelBuffer
{
public:
    PixelBuffer(int width, int height);
    // TODO: Overload operator[]? (would require separate type for second array)
    Color get(int x, int y);
    void set(int x, int y, Color color);
private:
    std::vector<Color> buffer;
    int computeIndex(int x, int y);
    int size;
};
