//
// Created by mikhail on 2/14/25.
//

#ifndef VIRTUAL_SCREEN_H
#define VIRTUAL_SCREEN_H
#include "math.h"

struct VirtualScreen {
    struct Vector2i size;
    float ratio;
    char *buffer;
};

struct VirtualScreen *NewVirtualScreen(char *buffer, int width, int height);

void InitBuffer(const struct VirtualScreen *screen);

void DrawLine(const struct VirtualScreen *screen, const struct Line *line);

#endif //VIRTUAL_SCREEN_H
