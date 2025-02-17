//
// Created by mikhail on 2/14/25.
//

#include "virtual_screen.h"

#include <curses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <bits/signum-generic.h>

#include "virtual_screen_data.h"
#include "debug.h"
#include "../math/matrix_op.h"
#include "../math/shape.h"

#define RENDER_COLOR 1
#define DEBUG_COLOR 2

static char *debugMessage;

static void GetCoordsByPixel(struct Vector2f *result, const struct VirtualScreenData *screen, int x, int y);

static void Dispose(int sig);

static void DrawLine(const struct VirtualScreenData *screen, const struct Line3f *line);

static void DrawBox(const struct VirtualScreenData *screen, const struct Box *cube);

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

void src_Init(struct VirtualScreenData *screen) {
    debug_Init();

    signal(SIGINT, Dispose);
    initscr();

    if (has_colors()) {
        start_color();

        init_pair(RENDER_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(DEBUG_COLOR, COLOR_WHITE, COLOR_RED);
    }

    getmaxyx(stdscr, screen->size.y, screen->size.x);
    screen->aspectRatio = (float) screen->size.y / (float) screen->size.x;

    struct Matrix4x4f matrix;
    mat4x4f_MakePerspective(&matrix, screen);
    screen->perspectiveMatrix = matrix;

    debugMessage = malloc(screen->size.x * sizeof(char));
}

void scr_InitRenderLoop(struct VirtualScreenData *screen) {
    float counter = 0.0f;
    screen->isRunning = true;

    while (screen->isRunning) {
        counter += 0.005f;
        struct Box cube = {0.1f, -0.05f, 1.0f, -0.1f, 0.05f, 0.3f, 0.05f};
        DrawBox(screen, &cube);

        move(0, 0);
        attrset(COLOR_PAIR(DEBUG_COLOR));
        addstr(debugMessage);
        refresh();
        usleep(screen->maxFrameRate);
        clear();
        attrset(COLOR_PAIR(RENDER_COLOR));
    }
    Dispose(0);
}

void pdm(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(debugMessage, format, args);
    va_end(args);
}

static void DrawLine(const struct VirtualScreenData *screen, const struct Line3f *line) {
    struct Vector4f p1;
    struct Vector4f p2;
    struct Line3f copiedLine = *line;

    // Make homogeneous
    vec3f_MakeHomogeneous(&p1, &copiedLine.p1);
    vec3f_MakeHomogeneous(&p2, &copiedLine.p2);

    // Apply perspective matrix
    mat4x4f_Mult(&p1, &screen->perspectiveMatrix, &p1);
    mat4x4f_Mult(&p2, &screen->perspectiveMatrix, &p2);

    // Divide by w
    vec4f_PerspectiveDivide(&copiedLine.p1, &p1);
    vec4f_PerspectiveDivide(&copiedLine.p2, &p2);

    // Temporary solution. Ignore depth (z)
    const struct Line2f line2f = {copiedLine.p1.x, copiedLine.p1.y, copiedLine.p2.x, copiedLine.p2.y, copiedLine.width};
    
    struct Rectangle rect;
    line_StretchToRectangle(&rect, &line2f);

    for (int rowIndex = 0; rowIndex < screen->size.y; ++rowIndex) {
        for (int horizontalOffset = 0; horizontalOffset < screen->size.x; ++horizontalOffset) {
            struct Vector2f pixelCoords;
            GetCoordsByPixel(&pixelCoords, screen, horizontalOffset, rowIndex);

            const float d1 = (rect.p2.x - rect.p1.x) * (pixelCoords.y - rect.p1.y) - (pixelCoords.x - rect.p1.x) *
                             (rect.p2.y - rect.p1.y);
            const float d2 = (rect.p3.x - rect.p2.x) * (pixelCoords.y - rect.p2.y) - (pixelCoords.x - rect.p2.x) *
                             (rect.p3.y - rect.p2.y);
            const float d3 = (rect.p4.x - rect.p3.x) * (pixelCoords.y - rect.p3.y) - (pixelCoords.x - rect.p3.x) *
                             (rect.p4.y - rect.p3.y);
            const float d4 = (rect.p1.x - rect.p4.x) * (pixelCoords.y - rect.p4.y) - (pixelCoords.x - rect.p4.x) *
                             (rect.p1.y - rect.p4.y);
            if (d1 >= 0 && d2 >= 0 && d3 >= 0 && d4 >= 0) {
                mvaddch(rowIndex, horizontalOffset, '0');
            }
        }
    }
}

static void DrawBoxSide(const struct VirtualScreenData *screen, struct Line3f *line, const float xDelta, const float yDelta) {
    line->p2 = line->p1;
    line->p2.y -= yDelta;
    DrawLine(screen, line);

    line->p1 = line->p2;
    line->p2.x -= xDelta;
    DrawLine(screen, line);

    line->p1 = line->p2;
    line->p2.y += yDelta;
    DrawLine(screen, line);

    line->p1 = line->p2;
    line->p2.x += xDelta;
    DrawLine(screen, line);
}

static void DrawBox(const struct VirtualScreenData *screen, const struct Box *cube) {
    const float xDelta = cube->topLeftFar.x - cube->botRightNear.x;
    const float yDelta = cube->topLeftFar.y - cube->botRightNear.y;
    const float zDelta = cube->topLeftFar.z - cube->botRightNear.z;
    struct Line3f line;
    line.width = cube->renderLineWidth;

    line.p1 = cube->topLeftFar;
    DrawBoxSide(screen, &line, xDelta, yDelta);
    line.p1 = cube->topLeftFar;
    line.p1.z -= zDelta;
    DrawBoxSide(screen, &line, xDelta, yDelta);

    line.p1 = cube->topLeftFar;
    line.p2 = line.p1;
    line.p2.z -= zDelta;
    DrawLine(screen, &line);
    line.p1.y -= yDelta;
    line.p2.y -= yDelta;
    DrawLine(screen, &line);
    line.p1 = cube->botRightNear;
    line.p2 = line.p1;
    line.p2.z += zDelta;
    DrawLine(screen, &line);
    line.p1.y += yDelta;
    line.p2.y += yDelta;
    DrawLine(screen, &line);
}

static void Dispose(int sig) {
    debug_Dispose();
    free(debugMessage);
    endwin();
    exit(0);
}

static void GetCoordsByPixel(struct Vector2f *result, const struct VirtualScreenData *screen, const int x, const int y) {
    const float screenWidthHalf = (float) screen->size.x / 2.0f;
    const float screenHeightHalf = (float) screen->size.y / 2.0f;
    const float newX = ((float) x - screenWidthHalf) * (1.0f / screenWidthHalf);
    const float newY = (screenHeightHalf - (float) y) * (1.0f / screenHeightHalf);
    result->x = newX;
    result->y = newY;
}
