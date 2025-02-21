//
// Created by mikhail on 2/17/25.
//

#include "shape.h"

#include "matrix.h"
#include "matrix_op.h"
#include "vector_op.h"

void line2f_StretchToRectangle(struct Box2f *result, const struct Line2f *line) {
    // Get line rotation
    struct Vector2f vectorBuffer;
    vec2f_SubVector(&vectorBuffer, &line->p2, &line->p1);
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
    box2f_Rotate(result, angle);

    // Move the rectangle
    vec2f_DivScalar(&vectorBuffer, &vectorBuffer, 2);
    struct Vector2f distance;
    vec2f_AddVector(&distance, &vectorBuffer, &line->p1);
    box2f_Translate(result, &distance);
}

void line4f_Transform(struct Line4f *line, const struct Matrix4x4f *matrix) {
    mat4x4f_MultVector(&line->p1, matrix, &line->p1);
    mat4x4f_MultVector(&line->p2, matrix, &line->p2);
}

void box2f_Translate(struct Box2f *box, const struct Vector2f *delta) {
    box->p1.x += delta->x;
    box->p1.y += delta->y;
    box->p2.x += delta->x;
    box->p2.y += delta->y;
    box->p3.x += delta->x;
    box->p3.y += delta->y;
    box->p4.x += delta->x;
    box->p4.y += delta->y;
}

void box2f_Rotate(struct Box2f *box, const float angle) {
    struct Matrix2x2f rotationMatrix;
    mat2x2f_MakeRotation(&rotationMatrix, angle);
    mat2x2f_Mult(&box->p1, &rotationMatrix, &box->p1);
    mat2x2f_Mult(&box->p2, &rotationMatrix, &box->p2);
    mat2x2f_Mult(&box->p3, &rotationMatrix, &box->p3);
    mat2x2f_Mult(&box->p4, &rotationMatrix, &box->p4);
}

void box4f_Bake(struct Box4f *box) {
    const float lengthHalf = box->length / 2;
    const float heightHalf = box->height / 2;
    const float widthHalf = box->width / 2;
    box->bakedP1.x = lengthHalf;
    box->bakedP1.y = -heightHalf;
    box->bakedP1.z = widthHalf;
    box->bakedP1.w = 1.0f;
    box->bakedP2.x = lengthHalf;
    box->bakedP2.y = -heightHalf;
    box->bakedP2.z = -widthHalf;
    box->bakedP2.w = 1.0f;
    box->bakedP3.x = -lengthHalf;
    box->bakedP3.y = -heightHalf;
    box->bakedP3.z = -widthHalf;
    box->bakedP3.w = 1.0f;
    box->bakedP4.x = -lengthHalf;
    box->bakedP4.y = -heightHalf;
    box->bakedP4.z = widthHalf;
    box->bakedP4.w = 1.0f;
    box->bakedP5.x = lengthHalf;
    box->bakedP5.y = heightHalf;
    box->bakedP5.z = widthHalf;
    box->bakedP5.w = 1.0f;
    box->bakedP6.x = lengthHalf;
    box->bakedP6.y = heightHalf;
    box->bakedP6.z = -widthHalf;
    box->bakedP6.w = 1.0f;
    box->bakedP7.x = -lengthHalf;
    box->bakedP7.y = heightHalf;
    box->bakedP7.z = -widthHalf;
    box->bakedP7.w = 1.0f;
    box->bakedP8.x = -lengthHalf;
    box->bakedP8.y = heightHalf;
    box->bakedP8.z = widthHalf;
    box->bakedP8.w = 1.0f;

    struct Matrix4x4f firstMatrix;
    struct Matrix4x4f secondMatrix;
    mat4x4f_MakeRotationX(&firstMatrix, box->rotation.x);
    mat4x4f_MakeRotationY(&secondMatrix, box->rotation.y);
    mat4x4f_MultMatrix(&box->transform, &secondMatrix, &firstMatrix);
    mat4x4f_MakeRotationZ(&firstMatrix, box->rotation.z);
    mat4x4f_MultMatrix(&box->transform, &firstMatrix, &box->transform);
    mat4x4f_MakeTranslation(&firstMatrix, &box->position);
    mat4x4f_MultMatrix(&box->transform, &firstMatrix, &box->transform);

    mat4x4f_MultVector(&box->bakedP1, &box->transform, &box->bakedP1);
    mat4x4f_MultVector(&box->bakedP2, &box->transform, &box->bakedP2);
    mat4x4f_MultVector(&box->bakedP3, &box->transform, &box->bakedP3);
    mat4x4f_MultVector(&box->bakedP4, &box->transform, &box->bakedP4);
    mat4x4f_MultVector(&box->bakedP5, &box->transform, &box->bakedP5);
    mat4x4f_MultVector(&box->bakedP6, &box->transform, &box->bakedP6);
    mat4x4f_MultVector(&box->bakedP7, &box->transform, &box->bakedP7);
    mat4x4f_MultVector(&box->bakedP8, &box->transform, &box->bakedP8);
}
