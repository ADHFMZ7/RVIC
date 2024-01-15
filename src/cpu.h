#ifndef CPU_H
#define CPU_H

#include "macros.h"

typedef struct core
{
	// registers
	u32 pc;
	u32 *x;
	
	// memory
	u8 *memory;

} core;

#endif
