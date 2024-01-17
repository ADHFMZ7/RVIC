#include <stdio.h>
#include "macros.h"

int main() {
    u32 x2 = -16;
    u32 x3 = 2;
    u32 x1;

    // Perform logical right shift
    x1 = (i32)x2 >> x3;

    printf("x1: %x\n", x1);

		x1 = x2 >> x3;
    printf("x1: %x\n", x1);


    return 0;
}

