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

void dump_registers(core *cpu);

void dump_memory(core *cpu);

u8 memory_read(core *cpu, u32 address);

void memory_write(core *cpu, u32 address, u8 value);

int load_file(const char *filename, u8 **buffer, u32 *size, u32 startIndex);

#endif
