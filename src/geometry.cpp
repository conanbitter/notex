#include "geometry.hpp"
#include <cmath>

using namespace notex;

bool Rect::contains(const Point& point)
{
    return
        point.x >= x &&
        point.y >= y &&
        point.x < x + width &&
        point.y < y + height;
}

Vec2D notex::Vec2D::rotate(float angle) {
    float c = std::cos(angle);
    float s = std::sin(angle);
    return Vec2D(
        x * c - y * s,
        x * s + y * c
    );
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

Vec2D Vec2D::operator-(const Vec2D& rhs)
{
    return Vec2D(x - rhs.x, y - rhs.y);
}

Vec2D& notex::Vec2D::operator-=(const Vec2D& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}

Vec2D& notex::Vec2D::operator*=(const Vec2D& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    return *this;
}

Vec2D notex::operator*(const Vec2D& vec, float val)
{
    return Vec2D(vec.x * val, vec.y * val);
}

Vec2D Transform::apply(const Vec2D& src)
{
    Vec2D res(src);
    if (origin) res -= *origin;
    if (angle) res = res.rotate(*angle);
    if (scale) res *= *scale;
    if (pos)res += *pos;

    return res;
}

void Transform::rotate(float rot_angle) {
    if (!angle) angle = 0.0f;
    *angle += rot_angle;
}

void Transform::enlarge(float mul_scale) {
    if (!scale) scale = Vec2D(1.0f, 1.0f);
    *scale *= Vec2D(mul_scale, mul_scale);
}

void Transform::move(float dx, float dy) {
    if (!pos) pos = Vec2D(0.0f, 0.0f);
    *pos += Vec2D(dx, dy);
}
