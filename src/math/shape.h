//
// Created by mikhail on 2/17/25.
//

#ifndef SHAPE_H
#define SHAPE_H

#include "vector.h"
#include "matrix.h"

struct Line2f {
    struct Vector2f p1;
    struct Vector2f p2;
    float width;
};

struct Line3f {
    struct Vector3f p1;
    struct Vector3f p2;
    float width;
};

struct Line4f {
    struct Vector4f p1;
    struct Vector4f p2;
    float width;
};

struct Box2f {
    struct Vector2f p1;
    struct Vector2f p2;
    struct Vector2f p3;
    struct Vector2f p4;
};

/**
 * Length - along X axis \n
 * Height - along Y axis \n
 * Width - along Z axis
 */
struct Box4f {
    float length;
    float height;
    float width;
    float wireFrameWidth;
    struct Vector3f position;
    struct Vector3f rotation;

    struct Matrix4x4f transform;
    struct Vector4f bakedP1;
    struct Vector4f bakedP2;
    struct Vector4f bakedP3;
    struct Vector4f bakedP4;
    struct Vector4f bakedP5;
    struct Vector4f bakedP6;
    struct Vector4f bakedP7;
    struct Vector4f bakedP8;
};

void line2f_StretchToRectangle(struct Box2f *result, const struct Line2f *line);

void line4f_Transform(struct Line4f *line, const struct Matrix4x4f *matrix);

void box2f_Translate(struct Box2f *box, const struct Vector2f *delta);

void box2f_Rotate(struct Box2f *box, float angle);

void box4f_Bake(struct Box4f *box);

#endif //SHAPE_H
