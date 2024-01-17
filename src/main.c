#include "macros.h"
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"

#define address_space 1024*1024*1024*4

// TODO:
// - Finish implementing instructions
// - Implement MMU
// - Think about optable with func pointers approach
//
// Goals far down the line:
// - Add pipelining
// - Add additional extensions
// - Create a debugger
// - Create an assembler/dissassembler

u32 extract_bits(u32 instruction, u8 start_bit, u8 num_bits) {
	u32 bitmask = ((1U << num_bits) - 1) << start_bit;
	return (instruction & bitmask) >> start_bit;
}

i32 main(i32 argc, byte **argv)
{

	// initialize memory space	

	core cpu;

	cpu.memory = (u8 *) malloc (sizeof(u8) * 1024);

	// Loads the instruction SRL x1, x2, x3
	cpu.memory[0] = 0x00;
	cpu.memory[1] = 0x31;
	cpu.memory[2] = 0x50;
	cpu.memory[3] = 0xb3;

	cpu.memory[4] = 0x40;
	cpu.memory[5] = 0x31;
	cpu.memory[6] = 0x50;
	cpu.memory[7] = 0xb3;

	// initialize registers
	cpu.pc = 0;

	cpu.x = (u32 *) malloc(sizeof(u32) * 32);
	cpu.x[0] = 0;

	// load program into memory

	cpu.x[2] = -16;
	cpu.x[3] = 2;

	// Make MMU so I dont have to allocate the entire addressable space

	// start fetch/execute/decode cycle

	u8 running = 2;

	while (running--)
	{
		// Make sure zero register is maintained.
		cpu.x[0] = 0;
		// fetch


		u32 instruction = 0;

		for (int i = 0; i < 4; i++) instruction += cpu.memory[cpu.pc++] << (8 * (3-i));
		
			printf("EXECUTING %x AT PC: %x\n", instruction, cpu.pc);
			
		// decode

		u8 opcode = extract_bits(instruction, 0, 7);
		u8 rd     = extract_bits(instruction, 7, 5);

		if (opcode == 0b0110011) // R Type 
		{
			u8 funct3 = extract_bits(instruction, 12, 3);
			u8 rs1    = extract_bits(instruction, 15, 5);
			u8 rs2    = extract_bits(instruction, 20, 5);
			u8 funct7 = extract_bits(instruction, 25, 7);

			if (funct3 == 0x0 && funct7 == 0x0)       // ADD 
				cpu.x[rd] = cpu.x[rs1] + cpu.x[rs2];
			else if (funct3 == 0x0 && funct7 == 0x20) // SUB
				cpu.x[rd] = cpu.x[rs1] - cpu.x[rs2];	
			else if (funct3 == 0x7 && funct7 == 0x0)  // AND
				cpu.x[rd] = cpu.x[rs1] & cpu.x[rs2];	
			else if (funct3 == 0x6 && funct7 == 0x0)  // OR
				cpu.x[rd] = cpu.x[rs1] | cpu.x[rs2];	
			else if (funct3 == 0x4 && funct7 == 0x0)  // XOR
				cpu.x[rd] = cpu.x[rs1] ^ cpu.x[rs2];	
			else if (funct3 == 0x1 && funct7 == 0x0)  // SLL
				cpu.x[rd] = cpu.x[rs1] << cpu.x[rs2];	
			else if (funct3 == 0x5 && funct7 == 0x0)  // SRL
				cpu.x[rd] = cpu.x[rs1] >> cpu.x[rs2];	
			else if (funct3 == 0x5 && funct7 == 0x20)  // SRA
				cpu.x[rd] = ((i32)cpu.x[rs1]) >> cpu.x[rs2];	
			else if (funct3 == 0x2 && funct7 == 0x0)  // SLT 
				cpu.x[rd] = (cpu.x[rs1] < cpu.x[rs2]) ? 0 : 1;	
			else if (funct3 == 0x3 && funct7 == 0x0)  // SLTU
				cpu.x[rd] = ((i32)cpu.x[rs1] < (i32)cpu.x[rs2]) ? 0 : 1;	
		}

		else if (opcode == 0b0010011) // I format 
		{
			u8 funct3 = extract_bits(instruction, 12, 3);
			u8 rs1    = extract_bits(instruction, 15, 5);
			u16 imm   = extract_bits(instruction, 20, 12);

			u8 imm_2  = extract_bits(instruction, 20, 5);
			u8 funct7 = extract_bits(instruction, 25, 7);

			if (funct3 == 0x0)												 // ADDI
				cpu.x[rd] = cpu.x[rs1] + imm;	
			else if (funct3 == 0x7)										 // ANDI
				cpu.x[rd] = cpu.x[rs1] & imm;	
			else if (funct3 == 0x6)										 // ORI 
				cpu.x[rd] = cpu.x[rs1] | imm;	
			else if (funct3 == 0x4)										 // XORI
				cpu.x[rd] = cpu.x[rs1] ^ imm;	
			else if (funct3 == 0x1)										 // SLLI
				cpu.x[rd] = cpu.x[rs1]  << imm_2;
			else if (funct3 == 0x5 && funct7 == 0x0)   // SRLI 
				cpu.x[rd] = cpu.x[rs1]  >> imm_2;
			else if (funct3 == 0x5 && funct7 == 0x20)  // SRAI 
				cpu.x[rd] = (i32) cpu.x[rs1]  >> imm_2;
			else if (funct3 == 0x5)										 // SLTI 
				cpu.x[rd] = (cpu.x[rs1] < imm) ? 1 : 0;
			else if (funct3 == 0x5)										 // SLTIU	 
				cpu.x[rd] = ((i32)cpu.x[rs1] < (i32) imm) ? 1 : 0;
		}
		else if (opcode == 0b0000011) // I format load
		{
			u8 funct3 = extract_bits(instruction, 12, 3);
			u8 rs1    = extract_bits(instruction, 15, 5);
			u16 imm   = extract_bits(instruction, 20, 12);

			if (funct3 == 0x0)
				

		}
		else if (opcode == 0b0100011) // 
		{


		}
		

		// execute

	}

}
