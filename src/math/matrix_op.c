//
// Created by mikhail on 2/17/25.
//

#include "matrix_op.h"

#include <math.h>

#include "vector_op.h"

static void mat4x4f_Init(struct Matrix4x4f *mat);

void mat2x2f_Mult(struct Vector2f *result, const struct Matrix2x2f *mat, const struct Vector2f *vec) {
    struct Vector2f xResult;
    struct Vector2f yResult;
    vec2f_MultScalar(&xResult, &mat->c1, vec->x);
    vec2f_MultScalar(&yResult, &mat->c2, vec->y);
    vec2f_AddVector(result, &xResult, &yResult);
}

void mat2x2f_MakeRotation(struct Matrix2x2f *mat, const float angle) {
    const float c = cosf(angle);
    const float s = sinf(angle);
    mat->c1.x = c;
    mat->c1.y = s;
    mat->c2.x = -s;
    mat->c2.y = c;
}

void mat4x4f_MultVector(struct Vector4f *result, const struct Matrix4x4f *mat, const struct Vector4f *vec) {
    struct Vector4f xV;
    struct Vector4f yV;
    struct Vector4f zV;
    struct Vector4f wV;
    vec4f_MultScalar(&xV, &mat->c1, vec->x);
    vec4f_MultScalar(&yV, &mat->c2, vec->y);
    vec4f_MultScalar(&zV, &mat->c3, vec->z);
    vec4f_MultScalar(&wV, &mat->c4, vec->w);
    vec4f_AddVector(result, &xV, &yV);
    vec4f_AddVector(result, result, &zV);
    vec4f_AddVector(result, result, &wV);
}

static void DoMatrix4x4fRowMult(struct Vector4f *result, const struct Matrix4x4f *left, const struct Vector4f *rightV) {
    struct Vector4f v1;
    struct Vector4f v2;
    struct Vector4f v3;
    struct Vector4f v4;
    vec4f_MultScalar(&v1, &left->c1, rightV->x);
    vec4f_MultScalar(&v2, &left->c2, rightV->y);
    vec4f_MultScalar(&v3, &left->c3, rightV->z);
    vec4f_MultScalar(&v4, &left->c4, rightV->w);
    vec4f_AddVector(result, &v1, &v2);
    vec4f_AddVector(result, result, &v3);
    vec4f_AddVector(result, result, &v4);
}

void mat4x4f_MultMatrix(struct Matrix4x4f *result, const struct Matrix4x4f *left, const struct Matrix4x4f *right) {
    DoMatrix4x4fRowMult(&result->c1, left, &right->c1);
    DoMatrix4x4fRowMult(&result->c2, left, &right->c2);
    DoMatrix4x4fRowMult(&result->c3, left, &right->c3);
    DoMatrix4x4fRowMult(&result->c4, left, &right->c4);
}

void mat4x4f_MakeTranslation(struct Matrix4x4f *mat, const struct Vector3f *delta) {
    mat4x4f_Init(mat);
    mat->c1.x = 1.0f;
    mat->c2.y = 1.0f;
    mat->c3.z = 1.0f;
    mat->c4.x = delta->x;
    mat->c4.y = delta->y;
    mat->c4.z = delta->z;
    mat->c4.w = 1.0f;
}

void mat4x4f_MakeRotationX(struct Matrix4x4f *mat, const float angle) {
    const float sin = sinf(angle);
    const float cos = cosf(angle);
    mat4x4f_Init(mat);
    mat->c1.x = 1.0f;
    mat->c2.y = cos;
    mat->c2.z = sin;
    mat->c3.y = -sin;
    mat->c3.z = cos;
    mat->c4.w = 1.0f;
}

void mat4x4f_MakeRotationY(struct Matrix4x4f *mat, const float angle) {
    const float sin = sinf(angle);
    const float cos = cosf(angle);
    mat4x4f_Init(mat);
    mat->c1.x = cos;
    mat->c1.z = -sin;
    mat->c2.y = 1.0f;
    mat->c3.x = sin;
    mat->c3.z = cos;
    mat->c4.w = 1.0f;
}

void mat4x4f_MakeRotationZ(struct Matrix4x4f *mat, const float angle) {
    const float sin = sinf(angle);
    const float cos = cosf(angle);
    mat4x4f_Init(mat);
    mat->c1.x = cos;
    mat->c1.y = sin;
    mat->c2.x = -sin;
    mat->c2.y = cos;
    mat->c3.z = 1.0f;
    mat->c4.w = 1.0f;
}

void mat4x4f_MakePerspective(struct Matrix4x4f *mat, const struct VirtualScreenData *screen) {
    const float n = 1.0f / tanf(screen->fov / 2.0f);
    const float fnRatio = screen->far / (screen->far - screen->near);
    mat4x4f_Init(mat);
    mat->c1.x = n * screen->aspectRatio;
    mat->c2.y = -n;
    mat->c3.z = fnRatio;
    mat->c3.w = 1.0f;
    mat->c4.z = -fnRatio * screen->near;
}

static void mat4x4f_Init(struct Matrix4x4f *mat) {
    const struct Vector4f c1 = {0.0f, 0.0f, 0.0f, 0.0f};
    const struct Vector4f c2 = {0.0f, 0.0f, 0.0f, 0.0f};
    const struct Vector4f c3 = {0.0f, 0.0f, 0.0f, 0.0f};
    const struct Vector4f c4 = {0.0f, 0.0f, 0.0f, 0.0f};
    mat->c1 = c1;
    mat->c2 = c2;
    mat->c3 = c3;
    mat->c4 = c4;
}
