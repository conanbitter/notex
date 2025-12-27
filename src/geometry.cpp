#include "geometry.hpp"

using namespace notex;

bool Rect::contains(const Point& point)
{
    return
        point.x >= x &&
        point.y >= y &&
        point.x < x + width &&
        point.y < y + height;
}

Vec2D Vec2D::operator+(const Vec2D& rhs)
{
    return Vec2D(x + rhs.x, y + rhs.y);
}

Vec2D& notex::Vec2D::operator+=(const Vec2D& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}

Vec2D notex::operator*(const Vec2D& vec, float val)
{
    return Vec2D(vec.x * val, vec.y * val);
}
