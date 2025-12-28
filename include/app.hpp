#pragma once

#include <string>
#include <unordered_set>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "graphics.hpp"
#include "keys.hpp"

namespace notex {
    enum class DrawingMode :GLenum {
        Tris = GL_TRIANGLES,
        Lines = GL_LINES,
        Points = GL_POINTS
    };

    class App {
    public:
        App() : m_init_complete{ false }, m_running{ false }, m_window{ nullptr } {};
        ~App();
        void init(const std::string& title, int width, int height, int initial_scale = 1);
        void run();

        void requestExit();

        void setCursorVisible(bool visible);
        bool getCursorVisible() const { return m_cursor_visible; };

        void setMode(DrawingMode mode);
        DrawingMode getMode() const { return m_mode; }
        void setLineWidth(float width);
        float getLineWidth() const { return m_line_width; }
        void setPointSize(float size);
        float getPointSize() const { return m_point_size; }
        void addVertex(const Vertex& vertex);
        void addVertexRaw(const Vertex& vertex);
        void addVertices(const std::vector<Vertex>& vertices);
        void addVertices(const std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
        void addIndices(std::vector<uint32_t>& indices);

        bool isKeyDown(Key key) const { return m_keys_down.contains(key); }
        bool isKeyPressed(Key key) const { return m_keys_pressed.contains(key); }
        bool isKeyReleased(Key key) const { return m_keys_released.contains(key); }

        virtual void load() {}
        virtual void update() {}
        virtual void draw() {}
    private:
        App(const App&) = delete;
        App& operator=(const App&) = delete;

        bool m_running;
        bool m_init_complete;
        bool m_cursor_visible;

        GLFWwindow* m_window;
        GraphicsContext m_context;
        Size m_view_size;

        DrawingMode m_mode;
        std::vector<Vertex> m_vertices;
        std::vector<uint32_t> m_indices;
        uint32_t m_index_offset;
        float m_line_width;
        float m_point_size;

        std::unordered_set<Key> m_keys_down;
        std::unordered_set<Key> m_keys_pressed;
        std::unordered_set<Key> m_keys_released;

        void flush();
        void clearBuffers();
        void resize(int new_width, int new_height);
        void keyAction(Key key, bool down);
        void keyClear();

        friend void resizeCallback(GLFWwindow* window, int width, int height);
        friend void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    };

}