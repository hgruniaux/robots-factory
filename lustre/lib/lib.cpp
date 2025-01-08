#include "lib.h"

#include <algorithm>
#include <cmath>
#include <numbers>

/*
 * Math functions
 */

void Lib__abs_step(float x, Lib__abs_out *out) {
    out->x = std::abs(x);
}

void Lib__sqrt_step(float x, Lib__sqrt_out *out) {
    out->x = std::sqrt(x);
}

void Lib__min_step(float x, float y, Lib__min_out *out) {
    out->x = std::min(x, y);
}

void Lib__max_step(float x, float y, Lib__max_out *out) {
    out->x = std::max(x, y);
}

void Lib__clamp_step(float x, float min, float max, Lib__clamp_out *out) {
    out->x = std::clamp(x, min, max);
}

static float our_fmod(float x, float y) {
    x = std::fmod(x, y);
    if (x < 0)
        x += y;
    return x;
}

void Lib__fmod_step(float x, float y, Lib__fmod_out *out) {
    out->x = our_fmod(x, y);
}

/*
 * Trigonometric functions
 */

static constexpr float deg2rad(float x) {
    return x * std::numbers::pi_v<float> / 180.0f;
}

static constexpr float rad2deg(float x) {
    return x * 180.0f / std::numbers::pi_v<float>;
}

void Lib__deg2rad_step(float x, Lib__deg2rad_out *out) {
    out->x = deg2rad(x);
}

void Lib__rad2deg_step(float x, Lib__rad2deg_out *out) {
    out->x = rad2deg(x);
}

void Lib__sin_step(float x, Lib__sin_out *out) {
    out->x = std::sin(deg2rad(x));
}

void Lib__cos_step(float x, Lib__cos_out *out) {
    out->x = std::cos(deg2rad(x));
}

void Lib__tan_step(float x, Lib__tan_out *out) {
    out->x = std::tan(deg2rad(x));
}

void Lib__asin_step(float x, Lib__asin_out *out) {
    out->x = rad2deg(std::asin(x));
}

void Lib__acos_step(float x, Lib__acos_out *out) {
    out->x = rad2deg(std::acos(x));
}

void Lib__atan_step(float x, Lib__atan_out *out) {
    out->x = rad2deg(std::atan(x));
}

/*
 * Arm control
 */

#define DEBUG_IK 0

void Lib__arm_end_effector_step(float theta1, float theta2, float theta3, float theta4,
                                float L1, float L2, float L3, float L4,
                                Lib__arm_end_effector_out *out) {
    // Convert to radians.
    theta1 = deg2rad(theta1);
    theta2 = deg2rad(theta2);
    theta3 = deg2rad(theta3);
    theta4 = deg2rad(theta4);

    // Calculate the end effector position.
    out->phi = theta1;
    out->y = L1 * std::cos(out->phi);
    out->x = -L1 * std::sin(out->phi);

    out->phi += theta2;
    out->y += L2 * std::cos(out->phi);
    out->x += -L2 * std::sin(out->phi);

    out->phi += theta3;
    out->y += L3 * std::cos(out->phi);
    out->x += -L3 * std::sin(out->phi);

    out->phi += theta4;
    out->y += L4 * std::cos(out->phi);
    out->x += -L4 * std::sin(out->phi);

    // Convert back to degrees.
    out->phi = our_fmod(rad2deg(out->phi), 360.0f);
}

#include <lbfgs.h>

static lbfgsfloatval_t ik_solver_evaluate(
        void *instance,
        const lbfgsfloatval_t *x,
        lbfgsfloatval_t *g,
        const int n,
        const lbfgsfloatval_t step) {
    const float *input = (const float *) instance;

    const auto theta1 = (float) x[0];
    const auto theta2 = (float) x[1];
    const auto theta3 = (float) x[2];
    const auto theta4 = (float) x[3];

    const float xe = input[0];
    const float ye = input[1];
    const float phi = input[2];

    const float L1 = input[3];
    const float L2 = input[4];
    const float L3 = input[5];
    const float L4 = input[6];

    auto cost = [=](const auto &effector) {
        const float dx = effector.x - xe;
        const float dy = effector.y - ye;
        // const float dphi = our_fmod(effector.phi - phi + 180.0f, 360.0f) - 180.0f;
        return dx * dx + dy * dy;
    };

    auto fk = [=](float t1, float t2, float t3, float t4) {
        Lib__arm_end_effector_out end_effector;
        Lib__arm_end_effector_step(t1, t2, t3, t4, L1, L2, L3, L4, &end_effector);
        return end_effector;
    };

    // Forward kinematics.
    const auto current_cost = cost(fk(theta1, theta2, theta3, theta4));

    // Compute the gradient.
    const float dtheta = 1e-3f;
    g[0] = (cost(fk(theta1 + dtheta, theta2, theta3, theta4)) - current_cost) / dtheta;
    g[1] = (cost(fk(theta1, theta2 + dtheta, theta3, theta4)) - current_cost) / dtheta;
    g[2] = (cost(fk(theta1, theta2, theta3 + dtheta, theta4)) - current_cost) / dtheta;
    g[3] = (cost(fk(theta1, theta2, theta3, theta4 + dtheta)) - current_cost) / dtheta;

    return current_cost;
}

#include <fmt/format.h>

static int ik_solver_progress(void *instance,
                              const lbfgsfloatval_t *x,
                              const lbfgsfloatval_t *g,
                              const lbfgsfloatval_t fx,
                              const lbfgsfloatval_t xnorm,
                              const lbfgsfloatval_t gnorm,
                              const lbfgsfloatval_t step,
                              int n,
                              int k,
                              int ls) {
#if DEBUG_IK
    fmt::print("Iteration {}:\n", k);
    fmt::print("  x = [{}, {}, {}, {}]\n", x[0], x[1], x[2], x[3]);
    fmt::print("  g = [{}, {}, {}, {}]\n", g[0], g[1], g[2], g[3]);
    fmt::print("  fx = {}\n", fx);
#endif
    return 0;
}

void Lib__arm_ik_step(float xe, float ye, float phi,
                      float theta1, float theta2, float theta3, float theta4,
                      float L1, float L2, float L3, float L4,
                      Lib__arm_ik_out *out) {
#if DEBUG_IK
    fmt::print("########### IK REQUEST\n");
    fmt::print("  Target = ({}, {}, {})\n", xe, ye, phi);
    fmt::print("  Current theta = ({}, {}, {}, {})\n", theta1, theta2, theta3, theta4);

    Lib__arm_end_effector_out end_effector;
    Lib__arm_end_effector_step(theta1, theta2, theta3, theta4, L1, L2, L3, L4, &end_effector);
    fmt::print("  Current end effector = ({}, {}, {})\n", end_effector.x, end_effector.y, end_effector.phi);
#endif

    constexpr int N = 4;
    lbfgsfloatval_t fx;
    lbfgsfloatval_t x[4] = {
            theta1,
            theta2,
            theta3,
            theta4};

    lbfgs_parameter_t param;
    lbfgs_parameter_init(&param);

    float input[7] = {
            xe,
            ye,
            phi,
            L1,
            L2,
            L3,
            L4};

    lbfgs(N, x, &fx, &ik_solver_evaluate, &ik_solver_progress, input, &param);
#if DEBUG_IK
    fmt::print("L-BFGS optimization terminated with cost = {}\n", fx);
    fmt::print("  x = [{}, {}, {}, {}]\n", x[0], x[1], x[2], x[3]);
    fmt::print("  fx = {}\n", fx);

    Lib__arm_end_effector_step(x[0], x[1], x[2], x[3], L1, L2, L3, L4, &end_effector);
    fmt::print("  End effector = ({}, {}, {})\n", end_effector.x, end_effector.y, end_effector.phi);
#endif

    out->theta1 = (float) x[0];
    out->theta2 = (float) x[1];
    out->theta3 = (float) x[2];
    out->theta4 = (float) x[3];
}
