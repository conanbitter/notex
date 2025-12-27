#pragma once

#include <glad/gl.h>
#include <vector>
#include "geometry.hpp"

namespace notex {
    class GraphicsContext {
    public:
        void init();
        void resize(int width, int height, Vec2D scale, Vec2D offset);
        void free();
        void draw(GLenum mode, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indexes);
    private:
        GLuint m_program;
        GLuint m_vao;
        GLuint m_vbo;
        GLuint m_ebo;
        GLint m_scale_uniform;
    };
}