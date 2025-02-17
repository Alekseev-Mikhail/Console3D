//
// Created by mikhail on 2/14/25.
//

#ifndef VIRTUAL_SCREEN_H
#define VIRTUAL_SCREEN_H

#include <stdbool.h>

#include "math.h"

struct VirtualScreen {
    struct Vector2i size;
    int maxFrameRate;
    float fov;
    float near;
    float far;
    float aspectRatio;
    bool isRunning;

    struct Matrix4x4f perspectiveMatrix;
};

struct VirtualScreen *scr_New(int maxFrameRate, float fov, float near, float far);

void src_Init(struct VirtualScreen *screen);

void scr_InitRenderLoop(struct VirtualScreen *screen);

/**
 * Push Debug Message
 */
void pdm(const char *format, ...);

#endif //VIRTUAL_SCREEN_H
