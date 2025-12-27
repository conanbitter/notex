#pragma once

#include <glad/gl.h>
#include "geometry.hpp"

namespace notex {
    class GraphicsContext {
    public:
        void init(int width, int height);
        void resize(int width, int height);
        void free();
        void present();
    private:
        GLuint m_program;
        GLuint m_vao;
        GLuint m_vbo;
        GLint m_scale_uniform;
    };
}