#pragma once

#include <stdint.h>

namespace notex {

    struct Vec2D
    {
        float x;
        float y;

        Vec2D(float x, float y) :x{ x }, y{ y } {}
        Vec2D() :x{ 0.0f }, y{ 0.0f } {}
    };

    typedef Vec2D Point;

    struct Size
    {
        float width;
        float height;

        Size(float width, float height) :width{ width }, height{ height } {}
        Size() :width{ 0.0f }, height{ 0.0f } {}
    };

    struct Rect
    {
        float x;
        float y;
        float width;
        float height;

        Rect(float x, float y, float width, float height) :x{ x }, y{ y }, width{ width }, height{ height } {}
        Rect(const Point& pos, const Size& size) :x{ pos.x }, y{ pos.y }, width{ size.width }, height{ size.height } {}
        Rect() :x{ 0.0f }, y{ 0.0f }, width{ 0.0f }, height{ 0.0f } {}

        Point pos() const { return Point(x, y); }
        Size size() const { return Size(width, height); }

        bool contains(const Point& point);
    };
}