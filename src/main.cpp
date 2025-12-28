#ifndef DEBUG
#include <windows.h>
#endif

#include "app.hpp"
#include "messages.hpp"
#include "color.hpp"
#include "keys.hpp"
#include "geometry.hpp"

class MyApp :public notex::App
{
public:
    MyApp() {}

    void load() override {
        pos = notex::Vec2D(35, 35);
        transform.pos = notex::Vec2D(35, 35);
        transform.origin = notex::Vec2D(25, 25);
    }

    void update() override {
        if (isKeyDown(notex::Key::Right)) {
            pos.x += 1.0f;
            transform.move(1.0f, 0.0f);
        }
        if (isKeyDown(notex::Key::Left)) {
            pos.x -= 1.0f;
            transform.move(-1.0f, 0.0f);
        }
        if (isKeyDown(notex::Key::Down)) {
            pos.y += 1.0f;
            transform.move(0.0f, 1.0f);
        }
        if (isKeyDown(notex::Key::Up)) {
            pos.y -= 1.0f;
            transform.move(0.0f, -1.0f);
        }

        col_mul += col_mul_d;
        if (col_mul <= 0.0 || col_mul >= 1.1) {
            col_mul_d = -col_mul_d;
        }

        s += ds;
        if (s <= 1.0 || s >= 2.0) {
            ds = -ds;
        }

        transform.enlarge(de);
        if (transform.scale->x <= 1.0) de = 1.01;
        if (transform.scale->x >= 2.0) de = 0.99;

        a += 0.01;
        transform.rotate(0.01);
    }

    void draw() override {

        std::vector<notex::Vertex> verts{
            notex::Vertex(0, 0, 1, 1, 1),
            notex::Vertex(50, 0, 1, 0, 0),
            notex::Vertex(50, 50, 0, 0, 1),
            notex::Vertex(0, 50, 0, 1, 0),
        };

        std::vector<uint32_t> inds{
            0,1,3,
            3,1,2
        };

        notex::Vec2D origin(25, 25);

        setMode(notex::DrawingMode::Tris);
        for (notex::Vertex vert : verts) {
            //addVertexRaw(notex::Vertex((vert.pos - origin).rotate(a) * s + pos, vert.color * col_mul));
            addVertexRaw(notex::Vertex(transform.apply(vert.pos), vert.color * col_mul));
        }
        addIndices(inds);
        //addVertices(verts, inds);

        setMode(notex::DrawingMode::Lines);
        setLineWidth(3.5f);
        addVertex(notex::Vertex(notex::Vec2D(200, 200), notex::Color(1.0, 1.0, 1.0)));
        addVertex(notex::Vertex(notex::Vec2D(400, 200), notex::Color(1.0, 0.5, 1.0)));
        addVertex(notex::Vertex(notex::Vec2D(400, 200), notex::Color(1.0, 0.5, 1.0)));
        addVertex(notex::Vertex(notex::Vec2D(300, 400), notex::Color(1.0, 1.0, 1.0)));

        setMode(notex::DrawingMode::Points);
        setPointSize(4.5f);
        addVertex(notex::Vertex(notex::Vec2D(500, 100), notex::Color(1.0, 1.0, 1.0)));
        addVertex(notex::Vertex(notex::Vec2D(500, 200), notex::Color(1.0, 0.5, 1.0)));
        addVertex(notex::Vertex(notex::Vec2D(550, 200), notex::Color(1.0, 0.5, 1.0)));
        addVertex(notex::Vertex(notex::Vec2D(600, 200), notex::Color(1.0, 1.0, 1.0)));
    }
private:
    notex::Vec2D pos;
    int x = 300;
    int y = 100;
    float col_mul = 1.0;
    float col_mul_d = -0.005;
    float a = 0.0;
    float s = 1.0;
    float ds = 0.01;
    float de = 1.01;
    notex::Transform transform;
};


#ifndef DEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
#else
int main(void) {
#endif
    try {
        MyApp app;
        app.init("Pixel Anvil", 640, 360, 1);
        app.run();
    }
    catch (const std::exception& e) {
        notex::msg::error(e.what());
    }
    catch (...) {
        notex::msg::error("Unknown error");
    }
    return 0;
}