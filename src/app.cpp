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

void App::init(const std::string& title, int width, int height, int initial_scale, bool use_integer_scaling) {
    if (m_init_complete) return;
    m_window = initWindow(title, width, height, initial_scale);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetWindowSizeCallback(m_window, resizeCallback);
    glfwSetKeyCallback(m_window, keyCallback);

    m_context.init(width, height);

    resize(width * initial_scale, height * initial_scale);

    load();
    m_init_complete = true;
}

void App::resize(int new_width, int new_height) {
    m_context.resize(new_width, new_height);
}

void App::run() {
    if (!m_init_complete) return;
    m_running = true;

    while (!glfwWindowShouldClose(m_window) && m_running) {
        update();
        keyClear();

        draw();

        glClear(GL_COLOR_BUFFER_BIT);

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