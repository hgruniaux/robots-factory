#include "lib.h"

#include <algorithm>
#include <cmath>
#include <numbers>

void Lib__abs_out_step(float x, Lib__abs_out *out) {
    out->x = std::abs(x);
}

void Lib__sqrt_out_step(float x, Lib__sqrt_out *out) {
    out->x = std::sqrt(x);
}

void Lib__min_out_step(float x, float y, Lib__min_out *out) {
    out->x = std::min(x, y);
}

void Lib__max_out_step(float x, float y, Lib__max_out *out) {
    out->x = std::max(x, y);
}

void Lib__clamp_out_step(float x, float min, float max, Lib__clamp_out *out) {
    out->x = std::clamp(x, min, max);
}

void Lib__fmod_out_step(float x, float y, Lib__fmod_out *out) {
    out->x = std::fmod(x, y);
}

static constexpr float deg2rad(float x) {
    return x * std::numbers::pi_v<float> / 180.0f;
}

static constexpr float rad2deg(float x) {
    return x * 180.0f / std::numbers::pi_v<float>;
}

void Lib__deg2rad_out_step(float x, Lib__deg2rad_out *out) {
    out->x = deg2rad(x);
}

void Lib__rad2deg_out_step(float x, Lib__rad2deg_out *out) {
    out->x = rad2deg(x);
}

void Lib__sin_out_step(float x, Lib__sin_out *out) {
    out->x = std::sin(deg2rad(x));
}

void Lib__cos_out_step(float x, Lib__cos_out *out) {
    out->x = std::cos(deg2rad(x));
}

void Lib__tan_out_step(float x, Lib__tan_out *out) {
    out->x = std::tan(deg2rad(x));
}

void Lib__asin_out_step(float x, Lib__asin_out *out) {
    out->x = rad2deg(std::asin(x));
}

void Lib__acos_out_step(float x, Lib__acos_out *out) {
    out->x = rad2deg(std::acos(x));
}

void Lib__atan_out_step(float x, Lib__atan_out *out) {
    out->x = rad2deg(std::atan(x));
}
