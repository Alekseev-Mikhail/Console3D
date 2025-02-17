#include <stdlib.h>

#include "virtual_screen.h"

int main(void) {
    struct VirtualScreen *screen = scr_New(60, 75.0f, 0.5f, 10.0f);

    src_Init(screen);
    scr_InitRenderLoop(screen);

    free(screen);
    return 0;
}
