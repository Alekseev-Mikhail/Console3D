#include <stdlib.h>

#include "virtual_screen.h"

int main(void) {
    const int width = 160;
    const int height = 48;
    struct VirtualScreen *screen = NewVirtualScreen(width, height, 60);

    src_Init();
    scr_InitRenderLoop(screen);

    free(screen);
    return 0;
}
