//
// Created by mikhail on 2/17/25.
//

#ifndef SHAPE_H
#define SHAPE_H
#include "vector.h"

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

struct Rectangle {
    struct Vector2f p1;
    struct Vector2f p2;
    struct Vector2f p3;
    struct Vector2f p4;
};

struct Box {
    struct Vector3f topLeftFar;
    struct Vector3f botRightNear;
    float renderLineWidth;
};

void line_StretchToRectangle(struct Rectangle *result, const struct Line2f *line);

void rec_Translate(struct Rectangle *rec, const struct Vector2f *by);

void rec_Rotate(struct Rectangle *rec, float angle);

#endif //SHAPE_H
