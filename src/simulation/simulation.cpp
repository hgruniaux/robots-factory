#include "simulation.hpp"
#include "parts/shape.hpp"
#include "world_description.hpp"
#include <spdlog/spdlog.h>

Simulation::Simulation(const std::shared_ptr<Robot> &robot, const std::shared_ptr<RobotAI> &robot_ai, const std::string &world_description)
    : m_robot(robot), m_robot_ai(robot_ai) {
    m_world = std::make_unique<b2World>(b2Vec2{DEFAULT_GRAVITY.x, DEFAULT_GRAVITY.y});

    // Try to load the world description if provided
    bool no_world_description_loaded = true;
    if (!world_description.empty()) {
        if (!load_world_description(m_world.get(), world_description)) {
            SPDLOG_ERROR("Failed to load world description from '{}'", world_description);
            m_world = std::make_unique<b2World>(b2Vec2{DEFAULT_GRAVITY.x, DEFAULT_GRAVITY.y});
        } else {
            no_world_description_loaded = false;
        }
    }

    b2Body *ground_body = nullptr;

    // Provide a default ground body if no world description was loaded
    if (no_world_description_loaded) {
        b2BodyDef ground_body_def;
        ground_body_def.type = b2_staticBody;
        ground_body = m_world->CreateBody(&ground_body_def);

        const float ground_half_width = 50.0f;
        const float ground_half_height = 50.0f;
        b2FixtureDef ground_fixture_def;
        b2PolygonShape ground_shape;
        ground_shape.SetAsBox(ground_half_width, ground_half_height, b2Vec2(0.0f, -ground_half_height), 0.0f);
        ground_fixture_def.shape = &ground_shape;
        ground_body->CreateFixture(&ground_fixture_def);
    } else if (m_world->GetBodyCount() > 0) {
        ground_body = m_world->GetBodyList();
    } else {
        b2BodyDef ground_body_def;
        ground_body_def.type = b2_staticBody;
        ground_body = m_world->CreateBody(&ground_body_def);
    }

    m_physics_robot = std::make_unique<PhysicsRobot>(robot, glm::vec2{0.f, 0.0f}, 0.f, m_world.get(), ground_body, true);

    if (m_robot_ai != nullptr)
        m_robot_ai->attach(m_robot, m_physics_robot.get());
}

Simulation::~Simulation() = default;

void Simulation::step() {
    m_world->Step(m_time_step, m_velocity_iterations, m_position_iterations);

    if (m_robot_ai != nullptr)
        m_robot_ai->step(m_time_step);

    m_elapsed_time += m_time_step;
}

void Simulation::draw(Renderer2D &renderer, bool debug) {
    if (debug)
        debug_draw(renderer);
    else
        normal_draw(renderer);
}

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

void Simulation::debug_draw(Renderer2D &renderer) {
    DebugDraw debug_draw{renderer};
    debug_draw.SetFlags(b2Draw::e_shapeBit | b2Draw::e_centerOfMassBit);
    m_world->SetDebugDraw(&debug_draw);
    m_world->DebugDraw();
    m_world->SetDebugDraw(nullptr);
}

void Simulation::normal_draw(Renderer2D &renderer) {
    b2Body *body = m_world->GetBodyList();
    while (body != nullptr) {
        auto user_data = body->GetUserData();
        if (user_data.pointer == 0) {
            body = body->GetNext();
            continue;
        }

        auto *shape = reinterpret_cast<Shape *>(user_data.pointer);
        DrawPartContext context;

        const b2Vec2 position = body->GetPosition();
        const float angle = body->GetAngle();
        shape->draw(renderer, context, {position.x, position.y}, glm::degrees(angle));

        body = body->GetNext();
    }
}
