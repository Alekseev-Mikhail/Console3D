//
// Created by mikhail on 2/17/25.
//

#ifndef MATRIX_OP_H
#define MATRIX_OP_H

#include "matrix.h"
#include "vector.h"
#include "../screen/virtual_screen_data.h"

void mat2x2f_Mult(struct Vector2f *result, const struct Matrix2x2f *mat, const struct Vector2f *vec);

void mat2x2f_MakeRotation(struct Matrix2x2f *mat, float angle);

void mat4x4f_MultVector(struct Vector4f *result, const struct Matrix4x4f *mat, const struct Vector4f *vec);

void mat4x4f_MultMatrix(struct Matrix4x4f *result, const struct Matrix4x4f *left, const struct Matrix4x4f *right);

void mat4x4f_MakeTranslation(struct Matrix4x4f *mat, const struct Vector3f *delta);

void mat4x4f_MakeRotationX(struct Matrix4x4f *mat, float angle);

void mat4x4f_MakeRotationY(struct Matrix4x4f *mat, float angle);

void mat4x4f_MakeRotationZ(struct Matrix4x4f *mat, float angle);

void mat4x4f_MakePerspective(struct Matrix4x4f *mat, const struct VirtualScreenData *screen);

#endif //MATRIX_OP_H
