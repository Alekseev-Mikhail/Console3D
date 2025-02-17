//
// Created by mikhail on 2/17/25.
//

#include "matrix_op.h"

#include <math.h>

void mat2x2f_Mult(struct Vector2f *result, const struct Matrix2x2f *mat, const struct Vector2f *vec) {
    const struct Vector2f xResult = {vec->x * mat->_11, vec->x * mat->_12};
    const struct Vector2f yResult = {vec->y * mat->_21, vec->y * mat->_22};
    vec2f_Add(result, &xResult, &yResult);
}

void mat2x2f_MakeRotation(struct Matrix2x2f *mat, const float angle) {
    const float c = cosf(angle);
    const float s = sinf(angle);
    mat->_11 = c;
    mat->_12 = s;
    mat->_21 = -s;
    mat->_22 = c;
}

void mat4x4f_Mult(struct Vector4f *result, const struct Matrix4x4f *mat, const struct Vector4f *vec) {
    const struct Vector4f xV = {vec->x * mat->_11, vec->x * mat->_12, vec->x * mat->_13, vec->x * mat->_14};
    const struct Vector4f yV = {vec->y * mat->_21, vec->y * mat->_22, vec->y * mat->_23, vec->y * mat->_24};
    const struct Vector4f zV = {vec->z * mat->_31, vec->z * mat->_32, vec->z * mat->_33, vec->z * mat->_34};
    const struct Vector4f wV = {vec->w * mat->_41, vec->w * mat->_42, vec->w * mat->_43, vec->w * mat->_44};
    vec4f_Add(result, &xV, &yV);
    vec4f_Add(result, result, &zV);
    vec4f_Add(result, result, &wV);
}

void mat4x4f_MakePerspective(struct Matrix4x4f *mat, const struct VirtualScreenData *screen) {
    const float n = 1.0f / tanf(screen->fov / 2.0f);
    const float fnRatio = screen->far / (screen->far - screen->near);

    mat->_11 = n * screen->aspectRatio;
    mat->_12 = 0.0f;
    mat->_13 = 0.0f;
    mat->_14 = 0.0f;
    mat->_21 = 0.0f;
    mat->_22 = n;
    mat->_23 = 0.0f;
    mat->_24 = 0.0f;
    mat->_31 = 0.0f;
    mat->_32 = 0.0f;
    mat->_33 = fnRatio;
    mat->_34 = 1.0f;
    mat->_41 = 0.0f;
    mat->_42 = 0.0f;
    mat->_43 = -fnRatio * screen->near;
    mat->_44 = 0.0f;
}
