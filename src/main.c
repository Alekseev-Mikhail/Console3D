#include <math.h>
#include <stdlib.h>

#include "screen/virtual_screen.h"
#include "math/shape.h"

void render(const struct VirtualScreenData *screen, float counter);

int main(void) {
    struct VirtualScreenData *screen = scr_New(60, 75.0f, 0.5f, 10.0f);

    src_Init(screen, 2.5f);
    scr_InitRenderLoop(screen, render);

    free(screen);
    return 0;
}

void render(const struct VirtualScreenData *screen, const float counter) {
    struct Box4f box = {0.2f, 0.2f, 0.2f, 0.05f};
    box.position.x = sinf(counter) * 0.1f;
    box.position.z = 1.0f;
    box.rotation.x = counter;
    box.rotation.y = counter * 2.0f;
    box.rotation.z = counter;

    src_DrawBox(screen, &box);
}
