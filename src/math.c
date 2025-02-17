//
// Created by mikhail on 2/14/25.
//

#include "math.h"

#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

struct Vector2f *vec2f_New(const float x, const float y) {
    struct Vector2f *ptr = malloc(sizeof(struct Vector2f));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Vector2f\n");
        abort();
    }
    ptr->x = x;
    ptr->y = y;
    return ptr;
}

struct Vector2i *vec2i_New(const int x, const int y) {
    struct Vector2i *ptr = malloc(sizeof(struct Vector2i));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Vector2i\n");
        abort();
    }
    ptr->x = x;
    ptr->y = y;
    return ptr;
}

struct Line2f *line_New(const struct Vector2f *start, const struct Vector2f *end, const float width) {
    struct Line2f *ptr = malloc(sizeof(struct Line2f));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Line\n");
        abort();
    }
    ptr->p1 = *start;
    ptr->p2 = *end;
    ptr->width = width;
    return ptr;
}

struct Rectangle *rec_New(const struct Vector2f *p1, const struct Vector2f *p2, const struct Vector2f *p3,
                          const struct Vector2f *p4) {
    struct Rectangle *ptr = malloc(sizeof(struct Rectangle));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Rectangle\n");
        abort();
    }
    ptr->p1 = *p1;
    ptr->p2 = *p2;
    ptr->p3 = *p3;
    ptr->p4 = *p4;
    return ptr;
}

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

void mat4x4f_MakePerspective(struct Matrix4x4f *mat, const float aspectRatio, const float fov, const float near, const float far) {
    const float n = 1.0f / tanf(fov / 2.0f);
    const float fnRatio = far / (far - near);

    mat->_11 = n * aspectRatio;
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
    mat->_43 = -fnRatio * near;
    mat->_44 = 0.0f;
}

void line_StretchToRectangle(struct Rectangle *result, const struct Line2f *line) {
    // Get line rotation
    struct Vector2f vectorBuffer;
    vec2f_Sub(&vectorBuffer, &line->p2, &line->p1);
    const float angle = vec2f_GetRotation(&vectorBuffer);

    // Set start point's positions
    const float distanceHalf = vec2f_DistanceTo(&line->p1, &line->p2) / 2;
    const float widthHalf = line->width / 2;

    result->p1.x = distanceHalf;
    result->p1.y = -widthHalf;
    result->p2.x = distanceHalf;
    result->p2.y = widthHalf;
    result->p3.x = -distanceHalf;
    result->p3.y = widthHalf;
    result->p4.x = -distanceHalf;
    result->p4.y = -widthHalf;

    // Rotate the rectangle
    rec_Rotate(result, angle);

    // Move the rectangle
    vec2f_DivideByScalar(&vectorBuffer, &vectorBuffer, 2);
    struct Vector2f distance;
    vec2f_Add(&distance, &vectorBuffer, &line->p1);
    rec_Translate(result, &distance);
}

void rec_Translate(struct Rectangle *rec, const struct Vector2f *by) {
    rec->p1.x += by->x;
    rec->p1.y += by->y;
    rec->p2.x += by->x;
    rec->p2.y += by->y;
    rec->p3.x += by->x;
    rec->p3.y += by->y;
    rec->p4.x += by->x;
    rec->p4.y += by->y;
}

void rec_Rotate(struct Rectangle *rec, const float angle) {
    struct Matrix2x2f rotationMatrix;
    mat2x2f_MakeRotation(&rotationMatrix, angle);
    mat2x2f_Mult(&rec->p1, &rotationMatrix, &rec->p1);
    mat2x2f_Mult(&rec->p2, &rotationMatrix, &rec->p2);
    mat2x2f_Mult(&rec->p3, &rotationMatrix, &rec->p3);
    mat2x2f_Mult(&rec->p4, &rotationMatrix, &rec->p4);
}
