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

#include "math.h"

static struct Vector2f *GetCoordsByPixel(const struct VirtualScreen *screen, int x, int y);

static void DrawLine(const struct VirtualScreen *screen, const struct Line *line);

struct VirtualScreen *NewVirtualScreen(const int width, const int height, const int framePerSec) {
    struct VirtualScreen *ptr = malloc(sizeof(struct VirtualScreen));
    if (ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory for Virtual Screen\n");
        abort();
    }
    const struct Vector2i size = {width, height};
    ptr->size = size;
    ptr->ratio = (float) width / (float) height;
    ptr->frameRate = 1000000 / framePerSec;
    ptr->isRunning = true;
    return ptr;
}

static void Dispose(int sig) {
    endwin();
    exit(0);
}

void src_Init() {
    (void) signal(SIGINT, Dispose); /* arrange interrupts to terminate */

    (void) initscr(); /* initialize the curses library */
    keypad(stdscr, TRUE); /* enable keyboard mapping */
    (void) nonl(); /* tell curses not to do NL->CR/NL on output */
    (void) cbreak(); /* take input chars one at a time, no wait for \n */
    (void) echo(); /* echo input - in color */
}

void scr_InitRenderLoop(const struct VirtualScreen *screen) {
    float counter = 0.0f;

    while (screen->isRunning) {
        counter += 0.1f;
        const struct Line line = {0.0f, 0.0f, 0.4f, powf(sinf(counter), 5) * 0.4f, 0.05f};
        const struct Line line1 = {0.1f, -0.3f, 0.4f, powf(sinf(counter), 5) * 0.4f, 0.05f};
        const struct Line line2 = {0.0f, 0.0f, 0.1f, -0.3f, 0.05f};
        DrawLine(screen, &line);
        DrawLine(screen, &line1);
        DrawLine(screen, &line2);

        move(0, 0);
        refresh();
        usleep(screen->frameRate);
        clear();
    }
    Dispose(0);
}

static void DrawLine(const struct VirtualScreen *screen, const struct Line *line) {
    struct Rectangle *rect = StretchLineToRectangle(line);

    for (int rowIndex = 0; rowIndex < screen->size.y; ++rowIndex) {
        for (int horizontalOffset = 0; horizontalOffset < screen->size.x; ++horizontalOffset) {
            struct Vector2f *pixelCoords = GetCoordsByPixel(screen, horizontalOffset, rowIndex);

            const float d1 = (rect->p2.x - rect->p1.x) * (pixelCoords->y - rect->p1.y) - (pixelCoords->x - rect->p1.x) *
                             (rect->p2.y - rect->p1.y);
            const float d2 = (rect->p3.x - rect->p2.x) * (pixelCoords->y - rect->p2.y) - (pixelCoords->x - rect->p2.x) *
                             (rect->p3.y - rect->p2.y);
            const float d3 = (rect->p4.x - rect->p3.x) * (pixelCoords->y - rect->p3.y) - (pixelCoords->x - rect->p3.x) *
                             (rect->p4.y - rect->p3.y);
            const float d4 = (rect->p1.x - rect->p4.x) * (pixelCoords->y - rect->p4.y) - (pixelCoords->x - rect->p4.x) *
                             (rect->p1.y - rect->p4.y);
            if (d1 >= 0 && d2 >= 0 && d3 >= 0 && d4 >= 0) {
                mvaddch(rowIndex, horizontalOffset, '0');
            }
            free(pixelCoords);
        }
    }
    free(rect);
}

static struct Vector2f *GetCoordsByPixel(const struct VirtualScreen *screen, const int x, const int y) {
    const float screenWidthHalf = (float) screen->size.x / 2.0f;
    const float screenHeightHalf = (float) screen->size.y / 2.0f;
    const float newX = ((float) x - screenWidthHalf) * (0.5f / screenWidthHalf) * screen->ratio;
    const float newY = (screenHeightHalf - (float) y) * (0.5f / screenHeightHalf);
    struct Vector2f *newCoords = NewVector2f(newX, newY);
    return newCoords;
}
