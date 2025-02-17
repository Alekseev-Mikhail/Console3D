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

struct Matrix2x2f {
    float _11;
    float _12;
    float _21;
    float _22;
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

struct Vector2f *vec2f_New(float x, float y);

struct Vector2i *vec2i_New(int x, int y);

struct Line *line_New(const struct Vector2f *start, const struct Vector2f *end, float width);

struct Rectangle *rec_New(const struct Vector2f *p1, const struct Vector2f *p2, const struct Vector2f *p3, const struct Vector2f *p4);

void vec2f_Add(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_Sub(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_DivideByScalar(struct Vector2f *result, const struct Vector2f *v, float scalar);

float vec2f_GetRotation(const struct Vector2f *v);

float vec2f_DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2);

void mat2x2f_Mult(struct Vector2f *result, const struct Matrix2x2f *mat, const struct Vector2f *vec);

void mat2x2f_MakeRotation(struct Matrix2x2f *mat, float angle);

void line_StretchToRectangle(struct Rectangle *result, const struct Line *line);

void rec_Translate(struct Rectangle *rec, const struct Vector2f *by);

void rec_Rotate(struct Rectangle *rec, float angle);

#endif //MATH_H
