#pragma once

#include <stdint.h>

namespace notex {

#pragma pack(push, 1)
    struct Color {
        float r;
        float g;
        float b;
        float a;

        Color() :r{ 0.0f }, g{ 0.0f }, b{ 0.0f }, a{ 1.0f } {}
        Color(float r, float g, float b, float a = 1.0f) :r{ r }, g{ g }, b{ b }, a{ a } {};

        static const Color BLACK;
        static const Color WHITE;
        static const Color RED;
        static const Color GREEN;
        static const Color BLUE;
        static const Color CYAN;
        static const Color MAGENTA;
        static const Color YELLOW;
        static const Color TRANSPARENT;

        Color operator*(const Color& rhs)const;
        Color operator*(float rhs)const;
    };
#pragma pack(pop)

}