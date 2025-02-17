//
// Created by mikhail on 2/14/25.
//

#include "math.h"

#include <math.h>
#include <stdio.h>
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

struct Line *line_New(const struct Vector2f *start, const struct Vector2f *end, const float width) {
    struct Line *ptr = malloc(sizeof(struct Line));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Line\n");
        abort();
    }
    ptr->start = *start;
    ptr->end = *end;
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

void line_StretchToRectangle(struct Rectangle *result, const struct Line *line) {
    // Get line rotation
    struct Vector2f vectorBuffer;
    vec2f_Sub(&vectorBuffer, &line->end, &line->start);
    const float angle = vec2f_GetRotation(&vectorBuffer);

    // Set start point's positions
    const float distanceHalf = vec2f_DistanceTo(&line->start, &line->end) / 2;
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
    vec2f_Add(&distance, &vectorBuffer, &line->start);
    rec_Translate(result, &distance);

    // const float dx = line->start.x - line->end.x;
    // const float dy = line->start.y - line->end.y;
    // const float widthHalf = line->width * 0.5f;//(line->width - r / 50) * 0.5f;
    // const float px = widthHalf * -dy;
    // const float py = widthHalf * dx;
    // const struct Vector2f p1 = {line->start.x - px, line->start.y - py};
    // const struct Vector2f p2 = {line->start.x + px, line->start.y + py};
    // const struct Vector2f p3 = {line->end.x + px, line->end.y + py};
    // const struct Vector2f p4 = {line->end.x - px, line->end.y - py};
    // result->p1 = p1;
    // result->p2 = p2;
    // result->p3 = p3;
    // result->p4 = p4;
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
