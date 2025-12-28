#pragma once

#include "geometry.hpp"
#include <optional>
#include <vector>

using std::optional;
using std::vector;

namespace notex {
    struct DrawingGroup {
        optional<float> size;
        vector<uint32_t> indices;
    };

    class Figure {
    private:
        vector<Vertex> vertices;
        vector<DrawingGroup> groups;

        friend class App;
    };

    const Figure test_figure;
}