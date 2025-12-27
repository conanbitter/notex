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
        pos = notex::Vec2D(10, 10);
    }

    void update() override {
        if (isKeyDown(notex::Key::Right)) {
            pos.x += 1.0f;
        }
        if (isKeyDown(notex::Key::Left)) {
            pos.x -= 1.0f;
        }
        if (isKeyDown(notex::Key::Down)) {
            pos.y += 1.0f;
        }
        if (isKeyDown(notex::Key::Up)) {
            pos.y -= 1.0f;
        }

        col_mul += col_mul_d;
        if (col_mul <= 0.0 || col_mul >= 1.1) {
            col_mul_d = -col_mul_d;
        }
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

        setMode(notex::DrawingMode::Tris);
        for (notex::Vertex vert : verts) {
            addVertexRaw(notex::Vertex(vert.pos + pos, vert.color * col_mul));
        }
        addIndices(inds);
        //addVertices(verts, inds);
    }
private:
    notex::Vec2D pos;
    int x = 300;
    int y = 100;
    float col_mul = 1.0;
    float col_mul_d = -0.01;
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