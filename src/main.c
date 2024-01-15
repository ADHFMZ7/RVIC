#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define address_space 1024*1024*1024*4

u32 extract_bits(u32 instruction, u8 start_bit, u8 num_bits) {
	u32 bitmask = ((1U << num_bits) - 1) << start_bit;
	return (instruction & bitmask) >> start_bit;
}

i32 main(i32 argc, byte **argv)
{

	// initialize memory space	

	core cpu;

	cpu.memory = (u8 *) malloc (sizeof(u8) * 1024);
	
	// memory[0] = 0b00000000;
	// memory[1] = 0b10110101;
	// memory[2] = 0b00000101;
	// memory[3] = 0b00111011;

	cpu.memory[0] = 0x3e;
	cpu.memory[1] = 0x80;
	cpu.memory[2] = 0x00;
	cpu.memory[3] = 0x93;

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


		u32 instruction = 0;

		for (int i = 0; i < 4; i++) instruction += cpu.memory[cpu.pc++] << (8 * (3-i));
			
		// decode

		u8 opcode = extract_bits(instruction, 0, 7);
		u8 rd     = extract_bits(instruction, 7, 5);

		printf("OP: %x\n", opcode);
		printf("RD: %x\n", rd);

		if (opcode == 0b0110011) // R Format
		{
			u8 funct3 = extract_bits(instruction, 12, 3);
			u8 rs1    = extract_bits(instruction, 15, 5);
			u8 rs2    = extract_bits(instruction, 20, 5);
			u8 funct7 = extract_bits(instruction, 25, 7);

		}
		else if (opcode == 0b0010011) // I format 
		{
			u8 funct3 = extract_bits(instruction, 12, 3);
			u8 rs1 = extract_bits(instruction, 15, 5);
			u16 imm = extract_bits(instruction, 20, 12);

			printf("%d, %d, %d, %d, %d\n", opcode, rd, funct3, rs1, imm);
		}
		else if (opcode == 0b0000011) // I format load
		{

		}
		else if (opcode == 0b01)
		{

		}
		

		// execute


		running = 0;
	}

}
