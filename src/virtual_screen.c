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

#include "debug.h"
#include "math.h"

#define RENDER_COLOR 1
#define DEBUG_COLOR 2

static char *debugMessage;

static void GetCoordsByPixel(struct Vector2f *result, const struct VirtualScreen *screen, int x, int y);

static void Dispose(int sig);

static void DrawLine(const struct VirtualScreen *screen, const struct Line *line);

struct VirtualScreen *scr_New(const int framePerSec) {
    struct VirtualScreen *ptr = malloc(sizeof(struct VirtualScreen));
    if (ptr == NULL) {
        LogAbort("ERROR: Cannot allocate memory for Virtual Screen\n");
        abort();
    }
    const struct Vector2i size = {-1, -1};
    ptr->size = size;
    ptr->frameRate = 1000000 / framePerSec;
    ptr->isRunning = false;
    return ptr;
}

void src_Init(struct VirtualScreen *screen) {
    debug_Init();

    (void) signal(SIGINT, Dispose); /* arrange interrupts to terminate */

    (void) initscr(); /* initialize the curses library */
    keypad(stdscr, TRUE); /* enable keyboard mapping */
    (void) nonl(); /* tell curses not to do NL->CR/NL on output */
    (void) cbreak(); /* take input chars one at a time, no wait for \n */
    (void) echo(); /* echo input - in color */

    if (has_colors()) {
        start_color();

        init_pair(RENDER_COLOR, COLOR_WHITE, COLOR_BLACK);
        init_pair(DEBUG_COLOR, COLOR_WHITE, COLOR_RED);
    }

    getmaxyx(stdscr, screen->size.y, screen->size.x);

    debugMessage = malloc(screen->size.x * sizeof(char));
}

void scr_InitRenderLoop(struct VirtualScreen *screen) {
    float counter = 0.0f;
    screen->isRunning = true;

    while (screen->isRunning) {
        counter += 0.005f;
        const struct Line line = {0.0f, 0.0f, 0.8f, sinf(counter) * 0.6f, 0.05f};
        const struct Line line1 = {0.2f, -0.7f, 0.8f, sinf(counter) * 0.6f, 0.05f};
        const struct Line line2 = {0.0f, 0.0f, 0.2f, -0.7f, 0.05f};
        struct Line line3 = {0.0f, 0.0f, 0.5f, 0.0f, 0.1f};
        struct Matrix2x2f mat;
        mat2x2f_MakeRotation(&mat, counter);
        mat2x2f_Mult(&line3.end, &mat, &line.end);
        DrawLine(screen, &line);
        DrawLine(screen, &line1);
        DrawLine(screen, &line2);
        DrawLine(screen, &line3);

        // struct Line line = {0.0f, 0.0f, -0.5f, 1.0f, 0.1f};
        // const struct Line line1 = {0.0f, 0.0f, 0.30f, 1.0f, 0.05f};
        // const struct Line line2 = {0.0f, 0.0f, 1.0f, 1.0f, 0.05f};
        // const struct Line line3 = {0.0f, 0.0f, 0.0f, 1.0f, 0.05f};
        // DrawLine(screen, &line);
        // DrawLine(screen, &line1);
        // DrawLine(screen, &line2);
        // DrawLine(screen, &line3);

        move(0, 0);
        attrset(COLOR_PAIR(DEBUG_COLOR));
        addstr(debugMessage);
        refresh();
        usleep(screen->frameRate);
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

static void DrawLine(const struct VirtualScreen *screen, const struct Line *line) {
    struct Rectangle rect;
    line_StretchToRectangle(&rect, line);

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

static void Dispose(int sig) {
    debug_Dispose();
    free(debugMessage);
    endwin();
    exit(0);
}

static void GetCoordsByPixel(struct Vector2f *result, const struct VirtualScreen *screen, const int x, const int y) {
    const float screenWidthHalf = (float) screen->size.x / 2.0f;
    const float screenHeightHalf = (float) screen->size.y / 2.0f;
    const float newX = ((float) x - screenWidthHalf) * (1.0f / screenWidthHalf);
    const float newY = (screenHeightHalf - (float) y) * (1.0f / screenHeightHalf);
    result->x = newX;
    result->y = newY;
}
