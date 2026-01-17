#include "app.hpp"
#include "messages.hpp"
#include <stdexcept>
#include <format>

using namespace notex;

static void notex::resizeCallback(GLFWwindow* window, int width, int height) {
    App* app = (App*)glfwGetWindowUserPointer(window);
    app->resize(width, height);
}

static void notex::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        App* app = (App*)glfwGetWindowUserPointer(window);
        app->keyAction(static_cast<Key>(key), action == GLFW_PRESS);
    }
}

static Size getScreenSize() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) msg::throwGLFWError();

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) msg::throwGLFWError();

    return Size(mode->width, mode->height);
}

static GLFWwindow* initWindow(const std::string& title, int width, int height, int scale) {
    if (!glfwInit()) {
        msg::throwGLFWError();
    }

    Size screen_size = getScreenSize();

    glfwWindowHint(GLFW_POSITION_X, (screen_size.width - width * scale) / 2);
    glfwWindowHint(GLFW_POSITION_Y, (screen_size.height - height * scale) / 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(width * scale, height * scale, title.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        notex::msg::throwGLFWError();
    }
    glfwSetWindowSizeLimits(window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE); notex::msg::checkGLFWError();

    glfwMakeContextCurrent(window);

    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }

#ifdef DEBUG
    msg::info(std::format("Loaded OpenGL {}.{}", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version)));
#endif

    glfwSwapInterval(1); msg::checkGLFWError();

    return window;
}

void App::init(const std::string& title, int width, int height, int initial_scale) {
    if (m_init_complete) return;
    m_window = initWindow(title, width, height, initial_scale);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, resizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);

    m_context.init();
    m_mode = DrawingMode::Tris;
    m_view_size = Size(width, height);
    m_global_scale = initial_scale;

    m_current_command.mode = DrawingMode::Tris;
    m_current_command.index_count = 0;
    m_current_command.index_offset = 0;
    m_current_command.size = -1.0f;
    m_current_command.vertex_offset = 0;

    resize(width * initial_scale, height * initial_scale);

    m_line_width = 1.0;
    m_next_line_width = -1.0;
    glLineWidth(m_line_width);
    glEnable(GL_LINE_SMOOTH);
    m_point_size = 1.0;
    m_next_point_size = -1.0;
    glPointSize(m_point_size);
    glDisable(GL_PROGRAM_POINT_SIZE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    load();
    m_init_complete = true;
}

void App::resize(int new_width, int new_height) {
    m_global_scale = std::min(
        (float)new_width / m_view_size.width,
        (float)new_height / m_view_size.height);
    Vec2D scale_data(
        2.0f / new_width * m_global_scale,
        2.0f / new_height * m_global_scale);
    Vec2D offset_data(
        1.0f - m_view_size.width * m_global_scale / new_width,
        1.0f - m_view_size.height * m_global_scale / new_height);
    m_context.resize(new_width, new_height, scale_data, offset_data);
    glLineWidth(m_line_width * m_global_scale);
    glPointSize(m_point_size * m_global_scale);
}

void App::run() {
    if (!m_init_complete) return;
    m_running = true;

    while (!glfwWindowShouldClose(m_window) && m_running) {
        update();
        keyClear();

        glClear(GL_COLOR_BUFFER_BIT);
        draw();
        flush();

        glfwSwapBuffers(m_window); msg::checkGLFWError();
        glfwPollEvents(); msg::checkGLFWError();
    }
}

App::~App() {
    glfwTerminate();
}

void App::requestExit() {
    m_running = false;
}

void App::setCursorVisible(bool visible) {
    m_cursor_visible = visible;
    if (visible) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    } else {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void App::keyAction(Key key, bool down) {
    if (down) {
        if (!m_keys_down.contains(key)) {
            m_keys_pressed.insert(key);
            m_keys_down.insert(key);
        }
    } else {
        if (m_keys_down.contains(key)) {
            m_keys_released.insert(key);
            m_keys_down.erase(key);
        }
    }
}

void App::keyClear() {
    m_keys_pressed.clear();
    m_keys_released.clear();
}

void App::flush()
{
    /*if (m_indices.empty()) return;
    m_context.draw((GLenum)m_mode, m_vertices, m_indices);
    clearBuffers();*/
    m_commands.push_back(m_current_command);
    m_current_command.index_count = 0;
    m_current_command.index_offset = m_indices.size();
    m_current_command.size = -1.0f;
    m_current_command.vertex_offset = m_vertices.size();
}

void App::clearBuffers() {
    m_vertices.clear();
    m_indices.clear();
    m_index_offset = 0;
}

void App::setMode(DrawingMode mode) {
    /*if (mode != m_mode) flush();
    m_mode = mode;*/
    if (m_current_command.mode == mode) return;
    if (m_current_command.index_count == 0) {
        if (m_current_command.mode == DrawingMode::Lines) m_next_line_width = m_current_command.size;
        if (m_current_command.mode == DrawingMode::Points) m_next_point_size = m_current_command.size;
    } else {
        flush();
    }
    m_current_command.mode = mode;
    if (mode == DrawingMode::Lines && m_next_line_width > 0.0f) {
        m_current_command.size = m_next_line_width;
        m_next_line_width = -1.0f;
    }
    if (mode == DrawingMode::Points && m_next_point_size > 0.0f) {
        m_current_command.size = m_next_point_size;
        m_next_point_size = -1.0f;
    }
}

void notex::App::setLineWidth(float width) {
    /*if (m_mode == DrawingMode::Lines) flush();
    m_line_width = width;
    glLineWidth(m_line_width * m_global_scale);*/
    if (m_current_command.mode == DrawingMode::Lines) {
        flush();
        m_current_command.mode = DrawingMode::Lines;
        m_current_command.size = width;
    } else {
        m_next_line_width = width;
    }
}

void notex::App::setPointSize(float size) {
    if (m_mode == DrawingMode::Points) flush();
    flush();
    m_point_size = size;
    glPointSize(m_point_size * m_global_scale);
}

void App::addVertex(const Vertex& vertex) {
    m_vertices.push_back(vertex);
    m_indices.push_back(m_index_offset);
    m_index_offset++;
}

void App::addVertexRaw(const Vertex& vertex) {
    m_vertices.push_back(vertex);
}

void App::addVertices(const std::vector<Vertex>& vertices) {
    for (auto vertex : vertices) {
        addVertex(vertex);
    }
}

void App::addVertices(const std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
    for (uint32_t index : indices) {
        m_indices.push_back(index + m_index_offset);
    }
    m_index_offset += m_indices.size();
}

void App::addIndices(std::vector<uint32_t>& indices) {
    for (uint32_t index : indices) {
        m_indices.push_back(index + m_index_offset);
    }
    m_index_offset += m_indices.size();
}
