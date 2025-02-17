#include <stdlib.h>

#include "screen/virtual_screen.h"

int main(void) {
    struct VirtualScreenData *screen = scr_New(60, 75.0f, 0.5f, 10.0f);

    src_Init(screen);
    scr_InitRenderLoop(screen);

    free(screen);
    return 0;
}
