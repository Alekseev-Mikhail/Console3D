//
// Created by mikhail on 2/14/25.
//

#ifndef MATH_H
#define MATH_H

struct Vector2f {
    float x;
    float y;
};

struct Vector2i {
    int x;
    int y;
};

struct Line {
    struct Vector2f start;
    struct Vector2f end;
    float width;
};

struct Rectangle {
    struct Vector2f p1;
    struct Vector2f p2;
    struct Vector2f p3;
    struct Vector2f p4;
};

struct Vector2f *NewVector2f(float x, float y);

struct Vector2i *NewVector2i(int x, int y);

struct Line *NewLine(const struct Vector2f *start, const struct Vector2f *end, float width);

struct Rectangle *NewRectangle(const struct Vector2f *p1, const struct Vector2f *p2, const struct Vector2f *p3, const struct Vector2f *p4);

void PrintVector2f(const struct Vector2f *v);

float DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2);

struct Rectangle *StretchLineToRectangle(const struct Line *line);

#endif //MATH_H
