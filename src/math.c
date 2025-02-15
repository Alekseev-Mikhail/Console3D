//
// Created by mikhail on 2/14/25.
//

#include "math.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Vector2f *NewVector2f(const float x, const float y) {
    struct Vector2f *ptr = malloc(sizeof(struct Vector2f));
    if (ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory for Vector2f\n");
        abort();
    }
    ptr->x = x;
    ptr->y = y;
    return ptr;
}

struct Vector2i *NewVector2i(const int x, const int y) {
    struct Vector2i *ptr = malloc(sizeof(struct Vector2i));
    if (ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory for Vector2i\n");
        abort();
    }
    ptr->x = x;
    ptr->y = y;
    return ptr;
}

struct Line *NewLine(const struct Vector2f *start, const struct Vector2f *end, const float width) {
    struct Line *ptr = malloc(sizeof(struct Line));
    if (ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory for Line\n");
        abort();
    }
    ptr->start = *start;
    ptr->end = *end;
    ptr->width = width;
    return ptr;
}

struct Rectangle *NewRectangle(const struct Vector2f *p1, const struct Vector2f *p2, const struct Vector2f *p3, const struct Vector2f *p4) {
    struct Rectangle *ptr = malloc(sizeof(struct Rectangle));
    if (ptr == NULL) {
        fprintf(stderr, "Cannot allocate memory for Rectangle\n");
        abort();
    }
    ptr->p1 = *p1;
    ptr->p2 = *p2;
    ptr->p3 = *p3;
    ptr->p4 = *p4;
    return ptr;
}

void PrintVector2f(const struct Vector2f *v) {
    printf("X: %f, Y: %f\n", v->x, v->y);
}

float DistanceTo(const struct Vector2f *v1, const struct Vector2f *v2) {
    return sqrtf(powf(v2->x - v1->x, 2) + powf(v2->y - v1->y, 2));
}

struct Rectangle *StretchLineToRectangle(const struct Line *line) {
    const float dx = line->start.x - line->end.x;
    const float dy = line->start.y - line->end.y;
    const float widthHalf = line->width * 0.5f;
    const float px = widthHalf * (-dy);
    const float py = widthHalf * dx;
    const struct Vector2f p1 = {line->start.x - px, line->start.y - py};
    const struct Vector2f p2 = {line->start.x + px, line->start.y + py};
    const struct Vector2f p3 = {line->end.x + px, line->end.y + py};
    const struct Vector2f p4 = {line->end.x - px, line->end.y - py};
    struct Rectangle *rect = NewRectangle(&p1, &p2, &p3, &p4);
    return rect;
}
