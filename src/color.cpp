#include "color.hpp"

#include <cmath>
#include <algorithm>

using namespace notex;

Color Color::operator*(const Color& rhs) const
{
    return Color(
        std::clamp(r * rhs.r, 0.0f, 1.0f),
        std::clamp(g * rhs.g, 0.0f, 1.0f),
        std::clamp(b * rhs.b, 0.0f, 1.0f),
        a
    );
}

Color Color::operator*(float rhs) const
{
    return Color(
        std::clamp(r * rhs, 0.0f, 1.0f),
        std::clamp(g * rhs, 0.0f, 1.0f),
        std::clamp(b * rhs, 0.0f, 1.0f),
        a
    );
}

const Color Color::BLACK = Color(0.0f, 0.0f, 0.0f);
const Color Color::WHITE = Color(1.0f, 1.0f, 1.0f);
const Color Color::RED = Color(1.0f, 0.0f, 0.0f);
const Color Color::GREEN = Color(0.0f, 1.0f, 0.0f);
const Color Color::BLUE = Color(0.0f, 0.0f, 1.0f);
const Color Color::CYAN = Color(0.0f, 1.0f, 1.0f);
const Color Color::MAGENTA = Color(1.0f, 0.0f, 1.0f);
const Color Color::YELLOW = Color(1.0f, 1.0f, 0.0f);