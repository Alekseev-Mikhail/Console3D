//
// Created by mikhail on 2/14/25.
//

#include "virtual_screen.h"

#include <stdio.h>
#include <stdlib.h>

#include "math.h"

struct VirtualScreen *NewVirtualScreen(char *buffer, const int width, const int height) {
    struct VirtualScreen *ptr = malloc(sizeof(struct VirtualScreen));
    if (ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory for Virtual Screen\n");
        abort();
    }
    const struct Vector2i size = {width, height};
    ptr->size = size;
    ptr->ratio = (float) width / (float) height;
    ptr->buffer = buffer;
    return ptr;
}

static struct Vector2f *GetCoordsByPixel(const struct VirtualScreen *screen, const int x, const int y) {
    const float screenWidthHalf = (float) screen->size.x / 2.0f;
    const float screenHeightHalf = (float) screen->size.y / 2.0f;
    const float newX = ((float) x - screenWidthHalf) * (0.5f / screenWidthHalf) * screen->ratio;
    const float newY = (screenHeightHalf - (float) y) * (0.5f / screenHeightHalf);
    struct Vector2f *newCoords = NewVector2f(newX, newY);
    return newCoords;
}

void InitBuffer(const struct VirtualScreen *screen) {
    printf("Initializing buffer...\n");

    for (int rowIndex = 0; rowIndex < screen->size.y; ++rowIndex) {
        const int verticalOffset = rowIndex * (screen->size.x + 1);
        for (int horizontalOffset = 0; horizontalOffset < screen->size.x; ++horizontalOffset) {
            screen->buffer[verticalOffset + horizontalOffset] = '0';
        }
        screen->buffer[verticalOffset + screen->size.x] = '\n';
    }

    printf("Buffer initialized\n");
}

void DrawLine(const struct VirtualScreen *screen, const struct Line *line) {
    struct Rectangle *rect = StretchLineToRectangle(line);

    printf("Drawing: Line\n");
    PrintVector2f(&rect->p1);
    PrintVector2f(&rect->p2);
    PrintVector2f(&rect->p3);
    PrintVector2f(&rect->p4);

    for (int rowIndex = 0; rowIndex < screen->size.y; ++rowIndex) {
        const int verticalOffset = rowIndex * (screen->size.x + 1);
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
                screen->buffer[verticalOffset + horizontalOffset] = ' ';
            }

            free(pixelCoords);
        }
    }

    free(rect);
}
