//
// Created by mikhail on 2/17/25.
//

#ifndef VIRTUAL_SCREEN_DATA_H
#define VIRTUAL_SCREEN_DATA_H

#include <stdbool.h>

#include "../math/vector.h"
#include "../math/matrix.h"

struct VirtualScreenData {
    struct Vector2i size;
    int maxFrameRate;
    float fov;
    float near;
    float far;
    float aspectRatio;
    bool isRunning;

    struct Matrix4x4f perspectiveMatrix;
};

#endif //VIRTUAL_SCREEN_DATA_H
