#include "macros.h"

// int main() {
//     u32 x2 = -16;
//     u32 x3 = 2;
//     u32 x1;
//
//     // Perform logical right shift
//     x1 = (i32)x2 >> x3;
//
//     printf("x1: %x\n", x1);
//
// 		x1 = x2 >> x3;
//     printf("x1: %x\n", x1);
//
//
//     return 0;
// }
//
#include <stdio.h>

int main() {
    unsigned int unsignedValue = 4294967295;  // Maximum value for unsigned int
    unsigned int signedValue = (int)unsignedValue;

    printf("Original unsigned value: %u\n", signedValue);
    printf("Converted signed value: %d\n", signedValue);

    return 0;
}

