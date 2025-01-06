#include "simulation.hpp"
#include <spdlog/spdlog.h>

class DebugDraw : public b2Draw {
public:
    explicit DebugDraw(Renderer2D &renderer) : m_renderer{renderer} {
    }

    void DrawPolygon(const b2Vec2 *vertices, int32 vertex_count, const b2Color &color) override {
        m_renderer.draw_polygon((const glm::vec2 *) vertices, vertex_count, {color.r, color.g, color.b, color.a});
    };

    void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertex_count, const b2Color &color) override {
        m_renderer.draw_solid_polygon((const glm::vec2 *) vertices, vertex_count, {color.r, color.g, color.b, color.a});
    };

    void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override {
        m_renderer.draw_circle({center.x, center.y}, radius, {color.r, color.g, color.b, color.a});
    };

    void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override {
        m_renderer.draw_solid_circle({center.x, center.y}, radius, {color.r, color.g, color.b, color.a});
    };

    void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override {
        m_renderer.draw_line({p1.x, p1.y}, {p2.x, p2.y}, 1.0f, {color.r, color.g, color.b, color.a});
    };

    void DrawTransform(const b2Transform &xf) override {
        b2Vec2 p1 = xf.p;
        b2Vec2 p2 = p1 + 0.5f * xf.q.GetXAxis();
        m_renderer.draw_line({p1.x, p1.y}, {p2.x, p2.y}, 1.0f, {1.0f, 0.0f, 0.0f, 1.0f});

        p2 = p1 + 0.5f * xf.q.GetYAxis();
        m_renderer.draw_line({p1.x, p1.y}, {p2.x, p2.y}, 1.0f, {0.0f, 1.0f, 0.0f, 1.0f});
    };

    void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override {
        m_renderer.draw_point({p.x, p.y}, size, {color.r, color.g, color.b, color.a});
    }

private:
    Renderer2D &m_renderer;
};// class DebugDraw

Simulation::Simulation(const std::shared_ptr<Robot> &robot, const std::shared_ptr<RobotAI> &robot_ai)
    : m_robot(robot), m_robot_ai(robot_ai) {
    b2BodyDef ground_body_def;
    ground_body_def.type = b2_staticBody;
    b2Body *ground_body = m_world.CreateBody(&ground_body_def);

    b2FixtureDef ground_fixture_def;
    b2PolygonShape ground_shape;
    ground_shape.SetAsBox(50.0f, 0.05f, b2Vec2(0.0f, -0.05f), 0.0f);
    ground_fixture_def.shape = &ground_shape;
    ground_body->CreateFixture(&ground_fixture_def);

    m_physics_robot = std::make_unique<PhysicsRobot>(robot, glm::vec2{0.f, 0.0f}, 0.f, &m_world, ground_body, true);

    if (m_robot_ai != nullptr)
        m_robot_ai->attach(m_robot, m_physics_robot.get());
}

Simulation::~Simulation() = default;

void Simulation::step() {
    m_world.Step(m_time_step, m_velocity_iterations, m_position_iterations);

    if (m_robot_ai != nullptr)
        m_robot_ai->step(m_time_step);

    m_elapsed_time += m_time_step;
}

void Simulation::draw(Renderer2D &renderer) {
    DebugDraw debug_draw{renderer};
    debug_draw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
    m_world.SetDebugDraw(&debug_draw);
    m_world.DebugDraw();
    m_world.SetDebugDraw(nullptr);
}
