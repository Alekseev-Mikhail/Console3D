//
// Created by mikhail on 2/14/25.
//

#include "virtual_screen.h"

#include <curses.h>
#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/signum-generic.h>

#include "virtual_screen_data.h"
#include "debug.h"
#include "../math/matrix_op.h"
#include "../math/vector_op.h"

#define RENDER_COLOR 1
#define DEBUG_COLOR 2

static char **debugMessages;
static int debugMessageCount = 0;

static void GetPixelCoords(struct Vector2f *result, const struct VirtualScreenData *screen, int x, int y);

static void Dispose(int sig);

struct VirtualScreenData *scr_New(const int maxFrameRate, const float fov, const float near, const float far) {
    struct VirtualScreenData *ptr = malloc(sizeof(struct VirtualScreenData));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Virtual Screen\n");
        abort();
    }
    const struct Vector2i size = {-1, -1};
    ptr->size = size;
    ptr->maxFrameRate = 1000000 / maxFrameRate;
    ptr->fov = fov;
    ptr->near = near;
    ptr->far = far;
    ptr->isRunning = false;
    return ptr;
}

void src_Init(struct VirtualScreenData *screen, const float yTermChar) {
    signal(SIGINT, Dispose);
    signal(SIGSEGV, Dispose);

    debug_Init();

    initscr();
    if (has_colors()) {
        start_color();

        init_pair(RENDER_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(DEBUG_COLOR, COLOR_WHITE, COLOR_RED);
    }

    getmaxyx(stdscr, screen->size.y, screen->size.x);
    screen->aspectRatio = (float) screen->size.y * yTermChar / (float) screen->size.x;

    debugMessages = malloc(screen->size.y * sizeof(char *));

    struct Matrix4x4f matrix;
    mat4x4f_MakePerspective(&matrix, screen);
    screen->perspectiveMatrix = matrix;
}

void scr_InitRenderLoop(struct VirtualScreenData *screen, void (*render)(const struct VirtualScreenData *, float)) {
    float counter = 0.0f;
    screen->isRunning = true;

    while (screen->isRunning) {
        counter += 0.02f;

        render(screen, counter);

        // Render debug messages
        attrset(COLOR_PAIR(DEBUG_COLOR));
        for (int i = 0; i < debugMessageCount; ++i) {
            mvaddstr(i, 0, debugMessages[i]);
        }
        move(0, 0);
        debugMessageCount = 0;

        // Refresh screen and wait for next frame
        refresh();
        usleep(screen->maxFrameRate);
        clear();
        attrset(COLOR_PAIR(RENDER_COLOR));
    }
    Dispose(0);
}

void pdm(const struct VirtualScreenData *screen, const char *format, ...) {
    if (debugMessages[debugMessageCount] != NULL) {
        free(debugMessages[debugMessageCount]);
    }
    va_list args;
    va_start(args, format);
    debugMessages[debugMessageCount] = malloc(screen->size.x * sizeof(char));
    vsprintf(debugMessages[debugMessageCount], format, args);
    debugMessageCount++;
    va_end(args);
}

void pdm_Vector4f(const struct VirtualScreenData *screen, const struct Vector4f *v) {
    pdm(screen, "X: %f, Y: %f, Z: %f, W: %f", v->x, v->y, v->z, v->w);
}

void src_DrawBox(const struct VirtualScreenData *screen, struct Box4f *box) {
    struct Line4f line;
    line.width = box->wireFrameWidth;
    box4f_Bake(box);

    src_DrawLineBetweenPoints(screen, &box->bakedP1, &box->bakedP2, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP2, &box->bakedP3, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP3, &box->bakedP4, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP4, &box->bakedP1, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP5, &box->bakedP6, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP6, &box->bakedP7, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP7, &box->bakedP8, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP8, &box->bakedP5, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP5, &box->bakedP1, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP6, &box->bakedP2, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP7, &box->bakedP3, box->wireFrameWidth);
    src_DrawLineBetweenPoints(screen, &box->bakedP8, &box->bakedP4, box->wireFrameWidth);
}

void src_DrawLine(const struct VirtualScreenData *screen, const struct Line4f *line) {
    src_DrawLineBetweenPoints(screen, &line->p1, &line->p2, line->width);
}

void src_DrawLineBetweenPoints(const struct VirtualScreenData *screen, const struct Vector4f *p1,
                                  const struct Vector4f *p2, const float width) {
    // Apply perspective matrix
    struct Vector4f copiedP1;
    struct Vector4f copiedP2;
    mat4x4f_MultVector(&copiedP1, &screen->perspectiveMatrix, p1);
    mat4x4f_MultVector(&copiedP2, &screen->perspectiveMatrix, p2);

    // Divide by w
    struct Line3f line3f;
    vec4f_PerspectiveDivide(&line3f.p1, &copiedP1);
    vec4f_PerspectiveDivide(&line3f.p2, &copiedP2);
    line3f.width = width;

    // Temporary solution. Ignore depth (z)
    const struct Line2f line2f = {line3f.p1.x, line3f.p1.y, line3f.p2.x, line3f.p2.y, line3f.width};

    // Stretch line to a box
    struct Box2f box;
    line2f_StretchToRectangle(&box, &line2f);

    // Find pixels, which are in the box
    for (int rowIndex = 0; rowIndex < screen->size.y; ++rowIndex) {
        for (int horizontalOffset = 0; horizontalOffset < screen->size.x; ++horizontalOffset) {
            struct Vector2f pixelCoords;
            GetPixelCoords(&pixelCoords, screen, horizontalOffset, rowIndex);

            const float d1 = (box.p2.x - box.p1.x) * (pixelCoords.y - box.p1.y) - (pixelCoords.x - box.p1.x) *
                             (box.p2.y - box.p1.y);
            const float d2 = (box.p3.x - box.p2.x) * (pixelCoords.y - box.p2.y) - (pixelCoords.x - box.p2.x) *
                             (box.p3.y - box.p2.y);
            const float d3 = (box.p4.x - box.p3.x) * (pixelCoords.y - box.p3.y) - (pixelCoords.x - box.p3.x) *
                             (box.p4.y - box.p3.y);
            const float d4 = (box.p1.x - box.p4.x) * (pixelCoords.y - box.p4.y) - (pixelCoords.x - box.p4.x) *
                             (box.p1.y - box.p4.y);
            if (d1 >= 0 && d2 >= 0 && d3 >= 0 && d4 >= 0) {
                mvaddch(rowIndex, horizontalOffset, '0');
            }
        }
    }
}

static void Dispose(int sig) {
    free(debugMessages);
    debug_Dispose();
    endwin();
    exit(0);
}

static void GetPixelCoords(struct Vector2f *result, const struct VirtualScreenData *screen, const int x, const int y) {
    const float screenWidthHalf = (float) screen->size.x / 2.0f;
    const float screenHeightHalf = (float) screen->size.y / 2.0f;
    const float newX = ((float) x - screenWidthHalf) * (1.0f / screenWidthHalf);
    const float newY = (screenHeightHalf - (float) y) * (1.0f / screenHeightHalf);
    result->x = newX;
    result->y = newY;
}
