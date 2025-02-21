//
// Created by mikhail on 2/17/25.
//

#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct Matrix2x2f {
    struct Vector2f c1;
    struct Vector2f c2;
};

struct Matrix4x4f {
    struct Vector4f c1;
    struct Vector4f c2;
    struct Vector4f c3;
    struct Vector4f c4;
};

#endif //MATRIX_H
