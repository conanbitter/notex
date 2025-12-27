#include "graphics.hpp"
#include <stdexcept>

using namespace std::string_literals;
using namespace notex;

#pragma region Shaders

static const char* vertex_shader_code = R"(
    #version 150
    uniform vec4 scale;

    in vec2 vert;
    in vec4 vertColor;

    out vec4 fragColor;

    void main() {
        gl_Position = vec4(vert.x*scale.x-1.0+scale.z, 1.0-vert.y*scale.y-scale.w, 0.0, 1.0);
        fragColor = vertColor;
    }
)";

static const char* fragment_shader_code = R"(
    #version 150
    uniform sampler2D tex;

    in vec4 fragColor;

    out vec4 outputColor;

    void main() {
        outputColor = fragColor;
    }
)";

static std::string getShaderLog(GLuint shader) {
    if (glIsShader(shader)) {
        std::string message = "";

        int info_log_length = 0;
        int max_length = info_log_length;

        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
        char* info_log = new char[max_length];

        glGetShaderInfoLog(shader, max_length, &info_log_length, info_log);
        if (info_log_length > 0) {
            message = info_log;
        }

        delete[] info_log;

        return message;
    } else {
        return "Wrong shader handle";
    }
}

static std::string getProgramLog(GLuint shader_program) {
    if (glIsProgram(shader_program)) {
        std::string message = "";

        int info_log_length = 0;
        int max_length = info_log_length;

        glGetProgramiv(shader_program, GL_INFO_LOG_LENGTH, &max_length);
        char* info_log = new char[max_length];

        glGetProgramInfoLog(shader_program, max_length, &info_log_length, info_log);
        if (info_log_length > 0) {
            message = info_log;
        }

        delete[] info_log;
        return message;
    } else {
        return "Wrong shader program handle";
    }
}

static GLuint compileShader(const char* source, GLenum shader_type) {
    GLuint shader = glCreateShader(shader_type);
    const GLchar* lines[] = { source };
    glShaderSource(shader, 1, lines, NULL);
    glCompileShader(shader);
    GLint is_compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled != GL_TRUE) {
        std::string msg;
        if (shader_type == GL_VERTEX_SHADER) {
            msg = "Vertex shader compile error: ";
        } else if (shader_type == GL_FRAGMENT_SHADER) {
            msg = "Fragment shader compile error: ";
        } else {
            msg = "Shader compile error: ";
        }
        throw std::runtime_error(msg + getShaderLog(shader));
    }
    return shader;
}

static GLuint compileShaderProgram(const char* vertex_shader, const char* fragment_shader) {
    GLuint program = glCreateProgram();

    GLuint compiled_vertex_shader = compileShader(vertex_shader, GL_VERTEX_SHADER);
    GLuint compiled_fragment_shader = compileShader(fragment_shader, GL_FRAGMENT_SHADER);

    glAttachShader(program, compiled_vertex_shader);
    glAttachShader(program, compiled_fragment_shader);
    glLinkProgram(program);
    GLint result = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        throw std::runtime_error("Shader link error: "s + getProgramLog(program));
    }
    glDeleteShader(compiled_vertex_shader);
    glDeleteShader(compiled_fragment_shader);
    return program;
};

static GLuint initShaders(GLint& vertex_pos, GLint& vertex_color, GLint& scale) {
    GLuint program = compileShaderProgram(vertex_shader_code, fragment_shader_code);
    vertex_pos = glGetAttribLocation(program, "vert");
    vertex_color = glGetAttribLocation(program, "vertColor");
    scale = glGetUniformLocation(program, "scale");
    return program;
}

static void scaleShaders(GLint param, Vec2D scale, Vec2D offset) {
    glUniform4f(param, scale.x, scale.y, offset.x, offset.y);
}

#pragma endregion Shaders

void GraphicsContext::init() {
    GLint vert_loc, vert_color_loc;
    m_program = initShaders(vert_loc, vert_color_loc, m_scale_uniform);

    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glEnableVertexAttribArray(vert_loc);
    glVertexAttribPointer(vert_loc, 2, GL_FLOAT, false, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(vert_color_loc);
    glVertexAttribPointer(vert_color_loc, 4, GL_FLOAT, false, sizeof(Vertex), (const void*)(2 * 4));

    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glUseProgram(m_program);
    //scaleShaders(m_scale_uniform, 0.5f, 0.5f);

    glClearColor(0.09, 0.14, 0.16, 1.0);
}

void GraphicsContext::resize(int width, int height, Vec2D scale, Vec2D offset) {
    glViewport(0, 0, width, height);
    scaleShaders(m_scale_uniform, scale, offset);
}

void GraphicsContext::free() {
    if (glIsBuffer(m_vbo)) {
        glDeleteBuffers(1, &m_vbo);
    }
    if (glIsVertexArray(m_vao)) {
        glDeleteVertexArrays(1, &m_vao);
    }
    if (glIsProgram(m_program)) {
        glDeleteProgram(m_program);
    }
}

void GraphicsContext::draw(GLenum mode, const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) {
    glUseProgram(m_program);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), indices.data(), GL_DYNAMIC_DRAW);

    //glDrawArrays(mode, 0, count);
    glDrawElements(mode, indices.size(), GL_UNSIGNED_INT, nullptr);
}