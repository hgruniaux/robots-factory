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

void Lib__fmod_step(float x, float y, Lib__fmod_out *out) {
    out->x = std::fmod(x, y);
    if (out->x < 0)
        out->x += y;
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

#include <glm/glm.hpp>

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
    out->x = L1 * std::cos(out->phi);
    out->y = L1 * std::sin(out->phi);

    out->phi += theta2;
    out->x += L2 * std::cos(out->phi);
    out->y += L2 * std::sin(out->phi);

    out->phi += theta3;
    out->x += L3 * std::cos(out->phi);
    out->y += L3 * std::sin(out->phi);

    out->phi += theta4;
    out->x += L4 * std::cos(out->phi);
    out->y += L4 * std::sin(out->phi);

    // Convert back to degrees.
    out->phi = rad2deg(out->phi);
}

static glm::mat3x4 compute_jacobian(float theta1, float theta2, float theta3, float theta4,
                                    float L1, float L2, float L3, float L4) {
    // Convert to degrees
    theta1 = deg2rad(theta1);
    theta2 = deg2rad(theta2);
    theta3 = deg2rad(theta3);
    theta4 = deg2rad(theta4);

    glm::mat3x4 J;

    // Position Jacobian (Partial derivatives of x and y with respect to each joint)
    J[0][0] = -L1 * std::sin(theta1) - L2 * std::sin(theta1 + theta2) - L3 * std::sin(theta1 + theta2 + theta3) - L4 * std::sin(theta1 + theta2 + theta3 + theta4);
    J[0][1] = -L2 * std::sin(theta1 + theta2) - L3 * std::sin(theta1 + theta2 + theta3) - L4 * std::sin(theta1 + theta2 + theta3 + theta4);
    J[0][2] = -L3 * std::sin(theta1 + theta2 + theta3) - L4 * std::sin(theta1 + theta2 + theta3 + theta4);
    J[0][3] = -L4 * std::sin(theta1 + theta2 + theta3 + theta4);

    J[1][0] = L1 * std::cos(theta1) + L2 * std::cos(theta1 + theta2) + L3 * std::cos(theta1 + theta2 + theta3) + L4 * std::cos(theta1 + theta2 + theta3 + theta4);
    J[1][1] = L2 * std::cos(theta1 + theta2) + L3 * std::cos(theta1 + theta2 + theta3) + L4 * std::cos(theta1 + theta2 + theta3 + theta4);
    J[1][2] = L3 * std::cos(theta1 + theta2 + theta3) + L4 * std::cos(theta1 + theta2 + theta3 + theta4);
    J[1][3] = L4 * std::cos(theta1 + theta2 + theta3 + theta4);

    // Orientation Jacobian (Partial derivatives of phi with respect to each joint angle)
    J[2][0] = 1;
    J[2][1] = 1;
    J[2][2] = 1;
    J[2][3] = 1;

    return J;
}

static glm::mat3x4 compute_pseudo_inverse(const glm::mat3x4 &J) {
    // Compute the pseudo-inverse of the Jacobian
    const auto Jt = glm::transpose(J);
    const auto JtJ = Jt * J;
    const auto JtJ_inv = glm::inverse(JtJ);
    return JtJ_inv * Jt;
}

void Lib__arm_ik_step(float xe, float ye, float phi,
                      float theta1, float theta2, float theta3, float theta4,
                      float L1, float L2, float L3, float L4,
                      Lib__arm_ik_out *out) {
    constexpr int MAX_ITERS = 10;
    constexpr float EPSILON = 0.01f;

    for (int i = 0; i < MAX_ITERS; ++i) {
        // Forward kinematics
        Lib__arm_end_effector_out end_effector;
        Lib__arm_end_effector_step(theta1, theta2, theta3, theta4, L1, L2, L3, L4, &end_effector);

        // Compute the error
        const float dx = xe - end_effector.x;
        const float dy = ye - end_effector.y;
        const float dphi = phi - end_effector.phi;
        const auto error = glm::vec3{dx, dy, dphi};
        if (glm::length(error) < EPSILON)
            break;

        // Compute the Jacobian
        const auto J = compute_jacobian(theta1, theta2, theta3, theta4, L1, L2, L3, L4);

        // Compute the pseudo-inverse of the Jacobian
        const auto J_inv = compute_pseudo_inverse(J);

        // Compute the change in joint angles
        const auto delta_theta = J_inv * error;

        // Update the joint angles
        theta1 += rad2deg(delta_theta.x);
        theta2 += rad2deg(delta_theta.y);
        theta3 += rad2deg(delta_theta.z);
        theta4 += rad2deg(delta_theta.w);
    }

    out->theta1 = theta1;
    out->theta2 = theta2;
    out->theta3 = theta3;
    out->theta4 = theta4;
}
