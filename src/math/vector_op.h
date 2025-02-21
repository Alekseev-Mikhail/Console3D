//
// Created by mikhail on 2/18/25.
//

#ifndef VECTOR_OP_H
#define VECTOR_OP_H

#include "vector.h"

void vec2f_AddVector(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_SubVector(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_MultScalar(struct Vector2f *result, const struct Vector2f *v, float scalar);

void vec2f_DivScalar(struct Vector2f *result, const struct Vector2f *v, float scalar);

float vec2f_GetRotation(const struct Vector2f *v);

float vec2f_DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2);

void vec3f_AddVector(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b);

void vec3f_SubVector(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b);

void vec3f_MakeHomogeneous(struct Vector4f *result, const struct Vector3f *v);

void vec4f_AddVector(struct Vector4f *result, const struct Vector4f *a, const struct Vector4f *b);

void vec4f_MultScalar(struct Vector4f *result, const struct Vector4f *v, float scalar);

void vec4f_PerspectiveDivide(struct Vector3f *result, const struct Vector4f *v);

#endif //VECTOR_OP_H
