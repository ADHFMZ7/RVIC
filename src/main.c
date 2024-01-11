#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define address_space 1024*1024*1024*4

i32 main(int argc, char **argv)
{

	// initialize memory space	

	core cpu;

	u8 *memory = (u8 *) malloc (sizeof(u8) * 1024);
	memory[0] = 0xFF;
	memory[1] = 0x00;
	memory[2] = 0xF1;
	memory[3] = 0x13;


	// initialize registers
	cpu.pc = 0;

	cpu.x = (u32 *) malloc(sizeof(u32) * 32);
	cpu.x[0] = 0;

	// load program into memory

	// start fetch/execute/decode cycle

	u8 running = 1;

	while (running)
	{
		// fetch


		// 0x FF 00 F1 13

		u32 instruction = 0;

		for (int i = 0; i < 4; i++) instruction += memory[cpu.pc++] << (8 * (3-i));
		
		u8 opcode = 0x7F & instruction;

		// decode


		// execute


		running = 0;
	}

}
