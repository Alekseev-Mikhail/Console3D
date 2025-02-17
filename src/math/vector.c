//
// Created by mikhail on 2/17/25.
//

#include "vector.h"

#include <math.h>

void vec2f_Add(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
}

void vec2f_Sub(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
}

void vec2f_DivideByScalar(struct Vector2f *result, const struct Vector2f *v, const float scalar) {
    result->x = v->x / scalar;
    result->y = v->y / scalar;
}

float vec2f_GetRotation(const struct Vector2f *v) {
    if (v->x == 0.0f) {
        return M_PI / 2;
    }
    return atanf(v->y / v->x);
}

float vec2f_DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2) {
    return sqrtf(powf(v2->x - v1->x, 2) + powf(v2->y - v1->y, 2));
}

void vec3f_Add(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
}

void vec3f_Sub(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b) {
    result->x = a->x - b->x;
    result->y = a->y - b->y;
    result->z = a->z - b->z;
}

void vec3f_MakeHomogeneous(struct Vector4f *result, const struct Vector3f *v) {
    result->x = v->x;
    result->y = v->y;
    result->z = v->z;
    result->w = 1.0f;
}

void vec4f_Add(struct Vector4f *result, const struct Vector4f *a, const struct Vector4f *b) {
    result->x = a->x + b->x;
    result->y = a->y + b->y;
    result->z = a->z + b->z;
    result->w = a->w + b->w;
}

void vec4f_PerspectiveDivide(struct Vector3f *result, const struct Vector4f *v) {
    result->x = v->x / v->w;
    result->y = v->y / v->w;
    result->z = v->z / v->w;
}
