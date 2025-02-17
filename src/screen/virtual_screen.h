//
// Created by mikhail on 2/14/25.
//

#ifndef VIRTUAL_SCREEN_H
#define VIRTUAL_SCREEN_H

struct VirtualScreenData *scr_New(int maxFrameRate, float fov, float near, float far);

void src_Init(struct VirtualScreenData *screen);

void scr_InitRenderLoop(struct VirtualScreenData *screen);

/**
 * Push Debug Message
 */
void pdm(const char *format, ...);

#endif //VIRTUAL_SCREEN_H
