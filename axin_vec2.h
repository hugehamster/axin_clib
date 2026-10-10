#ifndef AXIN_CLIB_VEC2_H
#define AXIN_CLIB_VEC2_H

#include <math.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    float x;
    float y;
} axin_vec2_t;

static const axin_vec2_t AXIN_VEC2_ZERO = {0, 0};
static const axin_vec2_t AXIN_VEC2_ONE = {1, 1};
static const axin_vec2_t AXIN_VEC2_UP = {0, -1};
static const axin_vec2_t AXIN_VEC2_DOWN = {0, 1};
static const axin_vec2_t AXIN_VEC2_LEFT = {-1, 0};
static const axin_vec2_t AXIN_VEC2_RIGHT = {1, 0};

static inline axin_vec2_t axin_vec2(
    float x,
    float y
) {
    return (axin_vec2_t){x, y};
}

static inline bool axin_vec2_is_zero(
    axin_vec2_t v
) {
    return v.x == 0 && v.y == 0;
}

static inline axin_vec2_t axin_vec2_add(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return axin_vec2(a.x + b.x, a.y + b.y);
}

static inline axin_vec2_t axin_vec2_sub(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return axin_vec2(a.x - b.x, a.y - b.y);
}

static inline axin_vec2_t axin_vec2_neg(
    axin_vec2_t v
) {
    return axin_vec2(-v.x, -v.y);
}

static inline axin_vec2_t axin_vec2_scale(
    axin_vec2_t v,
    float s
) {
    return axin_vec2(v.x * s, v.y * s);
}

static inline float axin_vec2_dot(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return a.x * b.x + a.y * b.y;
}

static inline float axin_vec2_cross(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return a.x * b.y - a.y * b.x;
}

static inline float axin_vec2_length_squared(
    axin_vec2_t v
) {
    return v.x * v.x + v.y * v.y;
}

static inline float axin_vec2_length(
    axin_vec2_t v
) {
    return hypotf(v.x, v.y);
}

static inline float axin_vec2_distance_squared(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return axin_vec2_length_squared(axin_vec2_sub(a, b));
}

static inline float axin_vec2_distance(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return axin_vec2_length(axin_vec2_sub(a, b));
}

static inline axin_vec2_t axin_vec2_normalize(
    axin_vec2_t v
) {
    if (v.x == 0 && v.y == 0) return AXIN_VEC2_ZERO;
    float m = fmaxf(fabsf(v.x), fabsf(v.y));
    axin_vec2_t tmp = axin_vec2(v.x / m, v.y / m);
    float l = hypotf(tmp.x, tmp.y);
    return (axin_vec2_t) { tmp.x / l, tmp.y / l};
}

static inline axin_vec2_t axin_vec2_direction(
    axin_vec2_t a,
    axin_vec2_t b
) {
    return axin_vec2_normalize(axin_vec2_sub(b, a));
}

static inline bool axin_vec2_near_equal(
    axin_vec2_t a,
    axin_vec2_t b,
    float epsilon
) {
    return fabsf(a.x - b.x) <= epsilon &&
        fabsf(a.y - b.y) <= epsilon;
}

static inline axin_vec2_t axin_vec2_lerp(
    axin_vec2_t a,
    axin_vec2_t b,
    float t
) {
    return axin_vec2_add(a, axin_vec2_scale(
        axin_vec2_sub(b, a), t));
}

static inline axin_vec2_t axin_vec2_perp(
    axin_vec2_t v
) {
    return axin_vec2(-v.y, v.x);
}

static inline axin_vec2_t axin_vec2_rotate(
    axin_vec2_t v,
    float radians
) {
    axin_vec2_t r;
    float c = cosf(radians);
    float s = sinf(radians);
    r.x = v.x * c - v.y * s;
    r.y = v.x * s + v.y * c;
    return r;
}

static inline float axin_vec2_angle(
    axin_vec2_t v
) {
    if (axin_vec2_is_zero(v)) return NAN;
    float angle = atan2f(v.y, v.x);
    return angle;
}

static inline axin_vec2_t axin_vec2_from_angle(
    float radians
) {
    return axin_vec2(cosf(radians), sinf(radians));
}

static inline float axin_vec2_signed_angle(
    axin_vec2_t a,
    axin_vec2_t b
) {
    const float pi = 3.14159265358979323846;
    float angle_a = axin_vec2_angle(a);
    float angle_b = axin_vec2_angle(b);
    if (isnan(angle_a) || isnan(angle_b)) return NAN;
    float d = angle_b - angle_a;
    if (d > pi) d -= 2 * pi;
    if (d < -pi) d += 2 * pi;
    return d;
}

static inline axin_vec2_t axin_vec2_project(
    axin_vec2_t v,
    axin_vec2_t onto
) {
    double ls = (double)onto.x * onto.x + (double)onto.y * onto.y;
    double dot = (double)v.x * onto.x + (double)v.y * onto.y;
    return axin_vec2(onto.x * dot / ls, onto.y * dot / ls);
}

static inline axin_vec2_t axin_vec2_reflect(
    axin_vec2_t v,
    axin_vec2_t normal
) {
    float dot = axin_vec2_dot(v, normal);
    return (axin_vec2_t){
        v.x - 2 * dot * normal.x,
        v.y - 2 * dot * normal.y
    };
}

static inline axin_vec2_t axin_vec2_move(
    axin_vec2_t from,
    axin_vec2_t to,
    float delta
) {
    axin_vec2_t x = axin_vec2_sub(to, from);
    float lx = axin_vec2_length(x);
    if (lx <= delta) return to;
    axin_vec2_t dir = axin_vec2_normalize(x);
    return axin_vec2(from.x + dir.x * delta, from.y + dir.y * delta);
}

#endif // AXIN_CLIB_VEC2_H
