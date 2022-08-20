#pragma once

#include <cinttypes>

namespace ogui
{
    struct Color
    {
        float r, g, b, a;
    };

    struct Rect
    {
        float x, y, w, h;
    };

    struct Vec2
    {
        float x, y;
    };

    struct Vertex
    {
        Vec2 position;
        Vec2 uv;
        uint32_t color;
    };
}
