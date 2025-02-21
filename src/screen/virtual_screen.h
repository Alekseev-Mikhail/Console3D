//
// Created by mikhail on 2/14/25.
//

#ifndef VIRTUAL_SCREEN_H
#define VIRTUAL_SCREEN_H

#include "../math/vector.h"
#include "../math/shape.h"

struct VirtualScreenData *scr_New(int maxFrameRate, float fov, float near, float far);

void src_Init(struct VirtualScreenData *screen, float yTermChar);

void scr_InitRenderLoop(struct VirtualScreenData *screen, void (*render)(const struct VirtualScreenData *, float));

void src_DrawBox(const struct VirtualScreenData *screen, struct Box4f *box);

void src_DrawLine(const struct VirtualScreenData *screen, const struct Line4f *line);

void src_DrawLineBetweenPoints(const struct VirtualScreenData *screen, const struct Vector4f *p1,
                                  const struct Vector4f *p2, float width);

/**
 * Push Debug Message
 */
void pdm(const struct VirtualScreenData *screen, const char *format, ...);

/**
 * Push Debug Message
 */
void pdm_Vector4f(const struct VirtualScreenData *screen, const struct Vector4f *v);

#endif //VIRTUAL_SCREEN_H
