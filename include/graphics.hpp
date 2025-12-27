#pragma once

#include <glad/gl.h>
#include "geometry.hpp"

namespace notex {
    class GraphicsContext {
    public:
        void init();
        void resize(int width, int height, Vec2D scale, Vec2D offset);
        void free();
        void draw(GLenum mode, Vertex* data, size_t count);
    private:
        GLuint m_program;
        GLuint m_vao;
        GLuint m_vbo;
        GLint m_scale_uniform;
    };
}