#include <stdio.h>
#include <stdlib.h>

#include "math.h"
#include "virtual_screen.h"

int main(void) {
    const int width = 100;
    const int height = 59;
    char *buffer = malloc(((width + 1) * height + 1) * sizeof(char));
    struct VirtualScreen *screen = NewVirtualScreen(buffer, width, height);
    InitBuffer(screen);

    // const struct Line line1 = {-0.6f, 0.45f, -0.f, -0.3f, 0.1f};
    const struct Line line2 = {-0.5f, 0.5f, 0.5f, -0.5f, 0.05f};
    // DrawLine(screen, &line1);
    DrawLine(screen, &line2);

    printf("%s", screen->buffer);

    free(buffer);
    free(screen);
    return 0;
}
