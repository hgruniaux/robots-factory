#include "lib.h"

void Lib__foo_step(float x, int y, Lib__foo_out *out) {
    out->x = x + y;
    out->y = x - y;
}
