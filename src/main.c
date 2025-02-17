#include <stdlib.h>

#include "virtual_screen.h"

int main(void) {
    struct VirtualScreen *screen = scr_New(60);

    src_Init(screen);
    scr_InitRenderLoop(screen);

    free(screen);
    return 0;
}
