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

struct Vector3f {
    float x;
    float y;
    float z;
};

struct Vector4f {
    float x;
    float y;
    float z;
    float w;
};

struct Matrix2x2f {
    float _11;
    float _12;
    float _21;
    float _22;
};

struct Matrix4x4f {
    float _11;
    float _12;
    float _13;
    float _14;
    float _21;
    float _22;
    float _23;
    float _24;
    float _31;
    float _32;
    float _33;
    float _34;
    float _41;
    float _42;
    float _43;
    float _44;
};

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

struct Vector2f *vec2f_New(float x, float y);

struct Vector2i *vec2i_New(int x, int y);

struct Line2f *line_New(const struct Vector2f *start, const struct Vector2f *end, float width);

struct Rectangle *rec_New(const struct Vector2f *p1, const struct Vector2f *p2, const struct Vector2f *p3, const struct Vector2f *p4);

void vec2f_Add(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_Sub(struct Vector2f *result, const struct Vector2f *a, const struct Vector2f *b);

void vec2f_DivideByScalar(struct Vector2f *result, const struct Vector2f *v, float scalar);

float vec2f_GetRotation(const struct Vector2f *v);

float vec2f_DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2);

void vec3f_Add(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b);

void vec3f_Sub(struct Vector3f *result, const struct Vector3f *a, const struct Vector3f *b);

void vec3f_MakeHomogeneous(struct Vector4f *result, const struct Vector3f *v);

void vec4f_Add(struct Vector4f *result, const struct Vector4f *a, const struct Vector4f *b);

void vec4f_PerspectiveDivide(struct Vector3f *result, const struct Vector4f *v);

void mat2x2f_Mult(struct Vector2f *result, const struct Matrix2x2f *mat, const struct Vector2f *vec);

void mat2x2f_MakeRotation(struct Matrix2x2f *mat, float angle);

void mat4x4f_Mult(struct Vector4f *result, const struct Matrix4x4f *mat, const struct Vector4f *vec);

void mat4x4f_MakePerspective(struct Matrix4x4f *mat, const float aspectRatio, const float fov, const float near, const float far);

void line_StretchToRectangle(struct Rectangle *result, const struct Line2f *line);

void rec_Translate(struct Rectangle *rec, const struct Vector2f *by);

void rec_Rotate(struct Rectangle *rec, float angle);

#endif //MATH_H
