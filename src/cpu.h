#ifndef CPU_H
#define CPU_H

#include "macros.h"
#include <stdio.h>

typedef struct core
{
	// registers
	u32 pc;
	u32 *x;
	
	// memory
	u8 *memory;

} core;

void dump_registers(core *cpu)
{
	for (int i = 0; i < 32; i++){
		printf("%d: %d\n", i, (i32)cpu->x[i]);
	}
}

#endif
