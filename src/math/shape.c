//
// Created by mikhail on 2/17/25.
//

#include "shape.h"

#include "matrix.h"
#include "matrix_op.h"

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
