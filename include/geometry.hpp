#pragma once

#include <stdint.h>
#include "color.hpp"
#include <optional>

namespace notex {

#pragma pack(push, 1)
    struct Vec2D {
        float x;
        float y;

        Vec2D(float x, float y) :x{ x }, y{ y } {}
        Vec2D() :x{ 0.0f }, y{ 0.0f } {}

        Vec2D rotate(float angle);
        Vec2D operator+(const Vec2D& rhs);
        Vec2D& operator+=(const Vec2D& rhs);
        Vec2D operator-(const Vec2D& rhs);
        Vec2D& operator-=(const Vec2D& rhs);
        Vec2D& operator*=(const Vec2D& rhs);
        friend Vec2D operator*(const Vec2D& vec, float val);
    };
#pragma pack(pop)

    typedef Vec2D Point;

    struct Size {
        float width;
        float height;

        Size(float width, float height) :width{ width }, height{ height } {}
        Size() :width{ 0.0f }, height{ 0.0f } {}
    };

    struct Rect {
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

#pragma pack(push, 1)
    struct Vertex {
        Vec2D pos;
        Color color;

        Vertex() :pos(), color() {}
        Vertex(const Vec2D& pos, const Color& color) :pos(pos), color(color) {}
        Vertex(float x, float y, float r, float g, float b, float a = 1.0f) :pos(x, y), color(r, g, b, a) {}
    };
#pragma pack(pop)

    struct Transform {
        std::optional<Vec2D> origin;
        std::optional<float> angle;
        std::optional<Vec2D> scale;
        std::optional<Vec2D> pos;

        Vec2D apply(const Vec2D& src);
        void rotate(float rot_angle);
        void enlarge(float mul_scale);
        void move(float dx, float dy);
    };
}