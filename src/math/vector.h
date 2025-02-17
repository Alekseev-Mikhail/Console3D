//
// Created by mikhail on 2/17/25.
//

#ifndef VECTOR_H
#define VECTOR_H

struct Vector2f {
    float x;
    float y;
};

struct Vector2i {
    int x;
    int y;
};

struct Vector3f {
    float x;
    float y;
    float z;
};

struct Vector4f {
    float x;
    float y;
    float z;
    float w;
};

void vec2f_Add(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_Sub(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_DivideByScalar(struct Vector2f *result, const struct Vector2f *v, float scalar);

float vec2f_GetRotation(const struct Vector2f *v);

float vec2f_DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2);

void vec3f_Add(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b);

void vec3f_Sub(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b);

void vec3f_MakeHomogeneous(struct Vector4f *result, const struct Vector3f *v);

void vec4f_Add(struct Vector4f *result, const struct Vector4f *a, const struct Vector4f *b);

void vec4f_PerspectiveDivide(struct Vector3f *result, const struct Vector4f *v);

#endif //VECTOR_H
