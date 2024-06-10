#include <iostream>
#include <vector>
#include <format>
#include <chrono>
#include <random>

using namespace std::chrono_literals;

#include <raylib.h>
#include <box2d/box2d.h>

#include <box2d_debug.h>

void CreateScene(b2World& world)
{
    auto rng = std::mt19937{ std::random_device{}() };
    auto vposDist = std::uniform_real_distribution<float>{ 10.0f, 118.0f };
    auto hposDist = std::uniform_real_distribution<float>{ 10.0f, 30.0f };
    auto angleDist = std::uniform_real_distribution<float>{ 0.0f, 3.14159f };
    auto sizeDist = std::uniform_real_distribution<float>{ 1.0f, 3.0f };

    {
        b2BodyDef bdef;
        b2PolygonShape shape;

        // Upper platform
        bdef.position = { 64.0f, 2.0f };
        shape.SetAsBox(60.0f, 1.0f);
        world.CreateBody(&bdef)->CreateFixture(&shape, 0.0f);

        // Bottom platform
        bdef.position = { 64.0f, 70.0f };
        shape.SetAsBox(60.0f, 1.0f);
        world.CreateBody(&bdef)->CreateFixture(&shape, 0.0f);

        // Left wall
        bdef.position = { 3.0f, 36.0f };
        shape.SetAsBox(0.5f, 32.0f);
        world.CreateBody(&bdef)->CreateFixture(&shape, 0.0f);

        // Right wall
        bdef.position = { 125.0f, 36.0f };
        shape.SetAsBox(0.5f, 32.0f);
        world.CreateBody(&bdef)->CreateFixture(&shape, 0.0f);

        // Middle mixer
        bdef.type = b2_kinematicBody;
        bdef.position = { 64.0f, 36.0f };
        shape.SetAsBox(32.0f, 0.5f);
        b2Body* body = world.CreateBody(&bdef);
        body->CreateFixture(&shape, 1.0f);
        body->SetAngularVelocity(1.0f);
    }

    // Falling squares
    for (size_t i = 0; i < 30; ++i)
    {
        b2BodyDef bdef;
        bdef.type = b2_dynamicBody;
        bdef.position = { vposDist(rng), hposDist(rng) };

        b2PolygonShape shape;
        shape.SetAsBox(sizeDist(rng), sizeDist(rng));

        world.CreateBody(&bdef)->CreateFixture(&shape, 1.0f);
    }

    // Falling circles
    for (size_t i = 0; i < 30; ++i)
    {
        b2BodyDef bdef;
        bdef.type = b2_dynamicBody;
        bdef.position = { vposDist(rng), hposDist(rng) };
        bdef.angle = angleDist(rng);

        b2CircleShape shape;
        shape.m_radius = sizeDist(rng);

        world.CreateBody(&bdef)->CreateFixture(&shape, 1.0f);
    }
}

struct Point {
    int x;
    int y;
};

bool IsContains(const Rectangle& rect, const Point& p) {
    return !(p.x < rect.x || p.y < rect.y || p.x > rect.x + rect.width || p.y > rect.y + rect.height);
}

class Quadtree {
   public:
    Quadtree(const Rectangle& rect, int cap) {
        _cap = cap;
        _rect = rect;
    }

    ~Quadtree() {
        if (_topLeft) delete _topLeft;
        if (_topRight) delete _topRight;
        if (_bottomLeft) delete _bottomLeft;
        if (_bottomRight) delete _bottomRight;
    }

    void Draw() {
        if (_isDiv) {
            _topLeft->Draw();
            _topRight->Draw();
            _bottomLeft->Draw();
            _bottomRight->Draw();
        } else {
            DrawRectangleLines(_rect.x, _rect.y, _rect.width, _rect.height, BLACK);
        }

        for (auto const& point : _points) {
            DrawCircle(point.x, point.y, 2, BLUE);
        }
    }

    bool insert(const Point& point) {
        if (!IsContains(_rect, point)) {
            return false;
        } else {
            if (!_isDiv) {
                if (_points.size() < _cap) {
                    _points.push_back(point);
                    return true;
                } else {
                    float w = _rect.width / 2;
                    float h = _rect.height / 2;
                    _topLeft = new Quadtree({_rect.x, _rect.y, w, h}, _cap);
                    _topRight = new Quadtree({_rect.x + w, _rect.y, w, h}, _cap);
                    _bottomLeft = new Quadtree({_rect.x, _rect.y + h, w, h}, _cap);
                    _bottomRight = new Quadtree({_rect.x + w, _rect.y + h, w, h}, _cap);
                    _isDiv = true;

                    for (auto&& ipoint : _points) {
                        bool isAdded = _topLeft->insert(ipoint) || _topRight->insert(ipoint) ||
                                       _bottomLeft->insert(ipoint) || _bottomRight->insert(ipoint);

                        if (!isAdded) {
                            std::cerr << "cant add point\n";
                        }
                    }

                    _points.clear();
                }
            }

            if (_topLeft->insert(point)) {
                return true;
            } else if (_topRight->insert(point)) {
                return true;
            } else if (_bottomLeft->insert(point)) {
                return true;
            } else if (_bottomRight->insert(point)) {
                return true;
            }

            return false;
        }
    }

   private:
    bool _isDiv = false;
    int _cap = 0;
    Rectangle _rect;
    std::vector<Point> _points;

    Quadtree* _topLeft = nullptr;
    Quadtree* _topRight = nullptr;
    Quadtree* _bottomLeft = nullptr;
    Quadtree* _bottomRight = nullptr;
};

int main(void) {
    float windW = 800;
    float windH = 450;

    SetTargetFPS(60);
    InitWindow(windW, windH, "Quadtree");

    b2DrawRayLib drawer{ 10.0f };
    drawer.SetFlags(
        b2Draw::e_shapeBit |
        b2Draw::e_jointBit |
        b2Draw::e_aabbBit |
        b2Draw::e_pairBit |
        b2Draw::e_centerOfMassBit
    );
    b2World world{ { 0.0f, 9.80665f } };
    world.SetDebugDraw(&drawer);
    CreateScene(world);

    Quadtree qtree({0, 0, windW, windH}, 4);

    auto preTime = std::chrono::system_clock::now();
    while (!WindowShouldClose()) {
        int fps = GetFPS();
        std::string strFps = std::format("FPS : {}", fps);

        auto const dt = GetFrameTime();
        world.Step(dt, 6, 2);
        
        BeginDrawing();
        {            
            ClearBackground(RAYWHITE);

            DrawText(strFps.c_str(), 0, 0, 20, BLACK);

            auto curTime = std::chrono::system_clock::now();

            if(curTime - preTime > 0.1s) {
                int x = GetRandomValue(0, windW);
                int y = GetRandomValue(0, windH);
                qtree.insert({x, y});

                preTime = curTime; 
            }
            
            qtree.Draw();

            
            world.DebugDraw();
        }
        EndDrawing();
    }

    CloseWindow();

    return 0;
}